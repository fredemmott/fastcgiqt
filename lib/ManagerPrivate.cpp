/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include "ManagerPrivate.h"

#include "Settings.h"
#include "SocketManager.h"

#include "fastcgi.h"

#include <QtEndian>
#include <QCoreApplication>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QHostAddress>
#include <QSocketNotifier>
#include <QTextStream>
#include <QThread>
#include <QTime>
#include <QTimer>

#include <errno.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/ip.h>

namespace FastCgiQt
{
	ManagerPrivate::ManagerPrivate(Responder::Generator responderGenerator, QObject* parent)
		:
			QObject(parent),
			m_responderGenerator(responderGenerator),
			m_applicationWatcher(new QFileSystemWatcher(this)),
			m_caches(new Caches())
	{
		// Check we're running as a FastCGI application
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);
		m_socket = FCGI_LISTENSOCK_FILENO;

		// The recommended way of telling if we're running as fastcgi or not.
		int error = ::getpeername(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		if(error == -1 && errno != ENOTCONN)
		{
			if(QCoreApplication::arguments().contains("--configure"))
			{
				configureHttpd();
				configureDatabase();
				exit(0);
			}
			if(QCoreApplication::arguments().contains("--configure-httpd"))
			{
				configureHttpd();
				exit(0);
			}
			if(QCoreApplication::arguments().contains("--configure-database"))
			{
				configureDatabase();
				exit(0);
			}
			Settings settings;
			if(settings.value("FastCGI/socketType", "FCGI-UNIX").toString() == "FCGI-TCP")
			{
				m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
				in_port_t port = settings.value("FastCGI/portNumber", 0).value<in_port_t>();
				if(port == 0)
				{
					qFatal("Configured to listen on TCP, but there isn't a valid Port Number configured. Try --configure-fastcgi");
					return;
				}
				sockaddr_in sa;
				::memset(&sa, 0, sizeof(sa));
				sa.sin_family = AF_INET;
				sa.sin_port = qToBigEndian(port);
				if(::bind(m_socket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) == -1)
				{
					qFatal("Failed to bind() to TCP port %d, with error %s", port, ::strerror(errno));
					return;
				}
				if(::listen(m_socket, 1) == -1)
				{
					qFatal("Failed to listen() on port %d, with error %s", port, ::strerror(errno));
					return;
				}
				QTextStream cout(stdout);
				cout << "Following configuration in '" << settings.fileName() << "' and listening on TCP port " << port << endl;
			}
			else
			{
				// Not a FastCGI application
				QTextStream cerr(stderr);
				cerr << "This application must be ran as a FastCGI application (eg from Apache via mod_fastcgi)." << endl;
				cerr << "Perhaps you wanted --configure?" << endl;
				exit(1);
				return;
			}
		}

		m_socketNotifier = new QSocketNotifier(m_socket, QSocketNotifier::Read, this),

		connect(
			m_socketNotifier,
			SIGNAL(activated(int)),
			this,
			SLOT(listen())
		);
		connect(
			m_applicationWatcher,
			SIGNAL(fileChanged(QString)),
			this,
			SLOT(shutdown())
		);
		m_applicationWatcher->addPath(QCoreApplication::applicationFilePath());

		// Spawn some threads
		for(int i = 0; i < qMax(QThread::idealThreadCount(), 1); ++i)
		{
			QThread* thread = new QThread(this);
			thread->start();
			m_threads.append(thread);
			m_threadLoads[thread] = 0;
		}

		// Wait for the event loop to start up before running
		QTimer::singleShot(0, this, SLOT(listen()));
	}

	ManagerPrivate::~ManagerPrivate()
	{
		delete m_caches;
	}

	QList<int> ManagerPrivate::threadLoads() const
	{
		QList<int> data;
		Q_FOREACH(const QAtomicInt& load, m_threadLoads)
		{
			data.append(load);
		}
		return data;
	}

	void ManagerPrivate::shutdown()
	{
		qDebug() << "Starting shutdown process";
		// stop listening on the main socket
		::close(m_socketNotifier->socket());
		// stop watching it
		m_socketNotifier->setEnabled(false);
		// If there's no load, exit
		exitIfFinished();
	}

	void ManagerPrivate::exitIfFinished()
	{
		if(m_socketNotifier->isEnabled())
		{
			return;
		}
		qDebug() << "Waiting for threads - thread loads:" << threadLoads();
		Q_FOREACH(QThread* thread, m_threads)
		{
			if(m_threadLoads.value(thread) != 0)
			{
				return;
			}
		}
		qDebug() << "Shutting down threads";
		Q_FOREACH(QThread* thread, m_threads)
		{
			thread->quit();
			bool done = thread->wait(10000);
			if(!done)
			{
				qDebug() << "One thread took longer than 10 seconds to shut down, terminating";
				thread->terminate();
			}
		}
		qDebug() << "Shutting down caches";
		delete m_caches;
		m_caches = NULL;
		qDebug() << "Shutdown complete. No thread load.";
		QCoreApplication::exit();
	}

	bool ManagerPrivate::hasLessLoadThan(QThread* t1, QThread* t2)
	{
		Q_ASSERT(t1->parent() == t2->parent());
		ManagerPrivate* p = qobject_cast<ManagerPrivate*>(t1->parent());
		Q_ASSERT(p);
		return p->m_threadLoads.value(t1) < p->m_threadLoads.value(t2);
	}

	void ManagerPrivate::listen()
	{
		// Initialise socket address structure
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);

		// Listen on the socket
		lockSocket(m_socket);
		int newSocket = ::accept(m_socket, reinterpret_cast<sockaddr*>(&sa), &len);
		releaseSocket(m_socket);

		/* We're connected, setup a SocketManager.
		 * This will delete itself when appropriate (via deleteLater())
		 */
		// Pick a thread to put it in
		qSort(m_threads.begin(), m_threads.end(), hasLessLoadThan);
		QThread* thread = m_threads.first();
		m_threadLoads[thread].ref();
		SocketManager* socket = new SocketManager(m_responderGenerator, newSocket, NULL);
		connect(
			socket,
			SIGNAL(finished(QThread*)),
			this,
			SLOT(reduceLoadCount(QThread*))
		);
		socket->moveToThread(thread);
	}

	void ManagerPrivate::reduceLoadCount(QThread* thread)
	{
		m_threadLoads[thread].deref();
		exitIfFinished();
	}

	void ManagerPrivate::configureHttpd()
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);

		Settings settings;
		settings.beginGroup("FastCGI");

		QString interface;

		cout << "*****************************************" << endl;
		cout << "***** FastCgiQt HTTPD Configuration *****" << endl;
		cout << "*****************************************" << endl;
		cout << "FastCgiQt supports two interfaces for communications with the HTTPD:" << endl;
		cout << "- FCGI-UNIX: Good for Apache with mod_fastcgi/mod_fcgid." << endl;
		cout << "   FastCgiQt tries to use the unix socket bound to file descriptor 0." << endl;
		cout << "   This is what the FastCGI specification says, but doesn't work too" << endl;
		cout << "   well with anything except Apache." << endl;
		cout << "- FCGI-TCP: Good for lighttpd, cherokee, and others." << endl;
		cout << "   FastCgiQt listens on a user-configured TCP port." << endl;
		cout << "   This works with pretty much anything that isn't Apache." << endl;
		cout << "Interface [FCGI-UNIX]: " << flush;
		interface = cin.readLine();
		if(interface.toUpper() == "FCGI-UNIX" || interface.isEmpty())
		{
			settings.setValue("socketType", "FCGI-UNIX");
		}
		else if(interface.toUpper() == "FCGI-TCP")
		{
			settings.setValue("socketType", "FCGI-TCP");
			QString portString;
			cout << "Port number: " << flush;
			portString = cin.readLine();
			bool ok;
			quint32 portNumber = portString.toUInt(&ok);
			if(!(ok && portNumber))
			{
				qFatal("Not a valid port number.");
				return;
			}
			settings.setValue("portNumber", portNumber);
		}
		else
		{
			qFatal("Not a valid communication method: '%s'", qPrintable(interface));
			return;
		}
		settings.sync();
	}

	void ManagerPrivate::configureDatabase()
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);
		QString driver;
		QString host;
		QString name;
		QString user;
		QString password;
		cout << "********************************************" << endl;
		cout << "***** FastCgiQt Database Configuration *****" << endl;
		cout << "********************************************" << endl;

		cout << "Driver [QMYSQL]: " << flush;
		driver = cin.readLine();
		if(driver.isEmpty())
		{
			driver = "QMYSQL";
		}

		cout << "Host [localhost]: " << flush;
		host = cin.readLine();
		if(host.isEmpty())
		{
			host = "localhost";
		}

		cout << "Database: " << flush;
		name = cin.readLine();

		cout << "User: " << flush;
		user = cin.readLine();

		cout << "Password: " << flush;
		password = cin.readLine();

		Settings settings;
		settings.beginGroup("database");
		settings.setValue("driver", driver);
		settings.setValue("host", host);
		settings.setValue("name", name);
		settings.setValue("user", user);
		settings.setValue("password", password);
		settings.endGroup();
		settings.sync();

		cout << "Settings saved in " << settings.fileName() << endl;
	}

	void ManagerPrivate::lockSocket(int socket)
	{
		::flock(socket, LOCK_EX);
	}

	void ManagerPrivate::releaseSocket(int socket)
	{
		::flock(socket, LOCK_UN);
	}
}
