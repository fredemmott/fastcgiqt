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

#include "CommunicationInterface.h"
#include "Settings.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QPluginLoader>
#include <QStringList>
#include <QTextStream>

namespace FastCgiQt
{
	ManagerPrivate::ManagerPrivate(Responder::Generator responderGenerator, QObject* parent)
		:
			QObject(parent),
			m_applicationWatcher(new QFileSystemWatcher(this)),
			m_caches(new Caches()),
			m_interface(0)
	{
		Q_FOREACH(QObject* object, QPluginLoader::staticInstances())
		{
			CommunicationInterface::Factory* factory(qobject_cast<CommunicationInterface::Factory*>(object));
			if(factory)
			{
				m_interface = factory->createInterface(responderGenerator, this);
				if(m_interface)
				{
					break;
				}
			}
		}

		if(!(m_interface && m_interface->start()))
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
			// Not a FastCGI application
			QTextStream cerr(stderr);
			cerr << "This application must be ran as a FastCGI application (eg from Apache via mod_fastcgi)." << endl;
			cerr << "Perhaps you wanted --configure?" << endl;
			exit(1);
			return;
		}

		connect(
			m_applicationWatcher,
			SIGNAL(fileChanged(QString)),
			this,
			SLOT(shutdown())
		);
		m_applicationWatcher->addPath(QCoreApplication::applicationFilePath());
	}

	ManagerPrivate::~ManagerPrivate()
	{
		delete m_caches;
		delete m_interface;
	}

	void ManagerPrivate::shutdown()
	{
		delete m_interface;
		m_interface =0;
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
}
