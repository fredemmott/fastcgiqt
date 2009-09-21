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
#include "FastCgiInterface.h"

#include "Settings.h"
#include "FastCgiSocketManager.h"

#include "fastcgi.h"

#include <QtEndian>
#include <QCoreApplication>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QHostAddress>
#include <QSocketNotifier>
#include <QTextStream>
#include <QThread>
#include <QTimer>

#include <errno.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/ip.h>

namespace FastCgiQt
{
	FastCgiInterface::FastCgiInterface(Responder::Generator responderGenerator, QObject* parent)
	: CommunicationInterface(parent)
	, m_responderGenerator(responderGenerator)
	{
	}

	bool FastCgiInterface::start()
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
			Settings settings;
			if(settings.value("FastCGI/socketType", "FCGI-UNIX").toString() == "FCGI-TCP")
			{
				m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
				in_port_t port = settings.value("FastCGI/portNumber", 0).value<in_port_t>();
				if(port == 0)
				{
					qFatal("Configured to listen on TCP, but there isn't a valid Port Number configured. Try --configure-fastcgi");
					return false;
				}
				sockaddr_in sa;
				::memset(&sa, 0, sizeof(sa));
				sa.sin_family = AF_INET;
				sa.sin_port = qToBigEndian(port);
				if(::bind(m_socket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) == -1)
				{
					qFatal("Failed to bind() to TCP port %d, with error %s", port, ::strerror(errno));
					return false;
				}
				if(::listen(m_socket, 1) == -1)
				{
					qFatal("Failed to listen() on port %d, with error %s", port, ::strerror(errno));
					return false;
				}
				QTextStream cout(stdout);
				cout << "Following configuration in '" << settings.fileName() << "' and listening on TCP port " << port << endl;
			}
			else
			{
				return false;
			}
		}

		m_socketNotifier = new QSocketNotifier(m_socket, QSocketNotifier::Read, this),

		connect(
			m_socketNotifier,
			SIGNAL(activated(int)),
			this,
			SLOT(listen())
		);

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

		return true;
	}

	FastCgiInterface::~FastCgiInterface()
	{
		shutdown();
	}

	QList<int> FastCgiInterface::threadLoads() const
	{
		QList<int> data;
		Q_FOREACH(const QAtomicInt& load, m_threadLoads)
		{
			data.append(load);
		}
		return data;
	}

	void FastCgiInterface::shutdown()
	{
		qDebug() << "Starting shutdown process";
		// stop listening on the main socket
		::close(m_socketNotifier->socket());
		// stop watching it
		m_socketNotifier->setEnabled(false);
		// If there's no load, exit
		exitIfFinished();
	}

	void FastCgiInterface::exitIfFinished()
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
		qDebug() << "Shutdown complete. No thread load.";
		QCoreApplication::exit();
	}

	bool FastCgiInterface::hasLessLoadThan(QThread* t1, QThread* t2)
	{
		Q_ASSERT(t1->parent() == t2->parent());
		FastCgiInterface* p = qobject_cast<FastCgiInterface*>(t1->parent());
		Q_ASSERT(p);
		return p->m_threadLoads.value(t1) < p->m_threadLoads.value(t2);
	}

	void FastCgiInterface::listen()
	{
		// Initialise socket address structure
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);

		// Listen on the socket
		lockSocket(m_socket);
		int newSocket = ::accept(m_socket, reinterpret_cast<sockaddr*>(&sa), &len);
		releaseSocket(m_socket);

		/* We're connected, setup a FastCgiSocketManager.
		 * This will delete itself when appropriate (via deleteLater())
		 */
		// Pick a thread to put it in
		qSort(m_threads.begin(), m_threads.end(), hasLessLoadThan);
		QThread* thread = m_threads.first();
		m_threadLoads[thread].ref();
		FastCgiSocketManager* socket = new FastCgiSocketManager(m_responderGenerator, newSocket, NULL);
		connect(
			socket,
			SIGNAL(finished(QThread*)),
			this,
			SLOT(reduceLoadCount(QThread*))
		);
		socket->moveToThread(thread);
	}

	void FastCgiInterface::reduceLoadCount(QThread* thread)
	{
		m_threadLoads[thread].deref();
		exitIfFinished();
	}

	void FastCgiInterface::lockSocket(int socket)
	{
		::flock(socket, LOCK_EX);
	}

	void FastCgiInterface::releaseSocket(int socket)
	{
		::flock(socket, LOCK_UN);
	}
}