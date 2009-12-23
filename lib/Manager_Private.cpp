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
#include "Manager_Private.h"

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
	Manager::Private::Private(QObject* parent)
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
				m_factories.append(factory);
			}
		}

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

		const QString interface = Settings().value("FastCGI/socketType", "FCGI-UNIX").toString();

		Q_FOREACH(CommunicationInterface::Factory* factory, m_factories)
		{
			m_interface = factory->createInterface(this);
			if(m_interface && m_interface->backends().contains(interface))
			{
				connect(
					m_interface,
					SIGNAL(newRequest(FastCgiQt::Request*)),
					this,
					SIGNAL(newRequest(FastCgiQt::Request*))
				);
				break;
			}
			else
			{
				delete m_interface;
				m_interface = 0;
			}
		}

		if(!(m_interface && m_interface->start(interface)))
		{
			// Not a FastCGI application
			QTextStream cerr(stderr);
			if(!m_interface)
			{
				cerr << "Could not find an implementation for backend " << interface <<endl;
			}
			else
			{
				cerr << "Failed to initialize backend " << interface << endl;
			}
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

	Manager::Private::~Private()
	{
		delete m_caches;
		delete m_interface;
	}

	void Manager::Private::shutdown()
	{
		delete m_interface;
		m_interface = 0;
	}

	void Manager::Private::configureHttpd()
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);

		Settings settings;
		settings.beginGroup("FastCGI");

		QStringList availableInterfaces;
		Q_FOREACH(CommunicationInterface::Factory* factory, m_factories)
		{
			CommunicationInterface* interface = factory->createInterface(this);
			availableInterfaces.append(interface->backends());
			delete interface;
		}
		qSort(availableInterfaces);

		cout << "*****************************************" << endl;
		cout << "***** FastCgiQt HTTPD Configuration *****" << endl;
		cout << "*****************************************" << endl;
		cout << "The following interfaces are available:" << endl;
		Q_FOREACH(const QString& interface, availableInterfaces)
		{
			cout << " - " << interface << endl;
		}
		cout << "Interface [FCGI-UNIX]: " << flush;

		QString interface;
		interface = cin.readLine().toUpper();
		if(interface.isEmpty())
		{
			interface = "FCGI-UNIX";
		}
		settings.setValue("socketType", interface);
		Q_FOREACH(CommunicationInterface::Factory* factory, m_factories)
		{
			m_interface = factory->createInterface(this);
			if(m_interface && m_interface->backends().contains(interface))
			{
				m_interface->configureHttpd(interface);
				break;
			}
			else
			{
				delete m_interface;
				m_interface = 0;
			}
		}

		if(m_interface)
		{
			delete m_interface;
		}
		else
		{
			qFatal("Not a valid communication method: '%s'", qPrintable(interface));
			return;
		}
		settings.sync();
	}

	void Manager::Private::configureDatabase()
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
