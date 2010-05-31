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
#include "HttpInterface.h"
#include "HttpRequest.h"

#include "Settings.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QStringList>
#include <QTcpServer>
#include <QTextStream>

namespace FastCgiQt
{
	HttpInterface::HttpInterface(QObject* parent)
	: CommunicationInterface(parent)
	, m_server(new QTcpServer(this))
	{
		connect(
			m_server,
			SIGNAL(newConnection()),
			SLOT(startResponse())
		);
	}

	QStringList HttpInterface::backends() const
	{
		return QStringList() << "HTTP";
	}

	bool HttpInterface::requiresConfiguration(const QString& backend) const
	{
		Q_ASSERT(backend == "HTTP");
		Q_UNUSED(backend);
		return false;
	}

	void HttpInterface::configureHttpd(const QString& backend)
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);
		Settings settings;
		settings.beginGroup("FastCGI");

		if(backend.toUpper() == "HTTP")
		{
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
			cout << "Optionally, you can specify a path relative to the current directory, where FastCgiQt will serve static content. This may be horribly insecure." << endl;
			cout << "Static content directory [none]: " << flush;
			const QString directory = cin.readLine().trimmed();
			if(directory.isEmpty())
			{
				settings.remove("staticDirectory");
			}
			else
			{
				settings.setValue("staticDirectory", QDir::cleanPath(directory));
			}
			return;
		}
		qFatal("Unknown HTTP backend: %s", qPrintable(backend));
	}

	bool HttpInterface::startBackend(const QString& backend)
	{
		if(backend != "HTTP")
		{
			return false;
		}
		const quint16 portNumber = Settings().value("FastCGI/portNumber", 0).value<quint16>();
		m_staticDirectory = Settings().value("FastCGI/staticDirectory", QString()).toString();
		QTextStream cout(stdout);
		if(!m_staticDirectory.isEmpty())
		{
			cout << "***WARNING*** Serving static content from ./" << m_staticDirectory << "/" << " (" << QFileInfo(m_staticDirectory).canonicalFilePath() << ")" << endl;
			cout << "Do *NOT* use this inefficient and insecure feature in a production setting, or any other environment" << endl;
			cout << "where untrusted users have access to this webserver." << endl;
		}
		if(!m_server->listen(QHostAddress::Any, portNumber))
		{
			qFatal("Failed to listen on port %d: %s", portNumber, qPrintable(m_server->errorString()));
		}
		if(portNumber != 0)
		{
			cout << "Using configuration in " << Settings().fileName() << ", and running an HTTP server on port " << portNumber << endl;
		}
		else
		{
			cout << "Auto-configured HTTP server running on port " << m_server->serverPort() << " - if this isn't what you want, re-run with --configure-httpd" << endl;
		}
		return true;
	}

	void HttpInterface::startResponse()
	{
		static ClientIODevice::HeaderMap standardHeaders;
		static QStringList staticDirectories;
		static QMutex mutex;
		{
			QMutexLocker lock(&mutex);
			if(standardHeaders.isEmpty())
			{
				standardHeaders.insert("SERVER_SOFTWARE", "FastCgiQt/HttpPlugin");
				standardHeaders.insert("SERVER_PORT", QString::number(m_server->serverPort()).toLatin1());
				standardHeaders.insert("GATEWAY_INTERFACE", "CGI/1.1");
				if(!m_staticDirectory.isEmpty())
				{
					staticDirectories.append(QFileInfo(m_staticDirectory).canonicalFilePath());
				}
			}
		}
		connect(
			new HttpRequest(standardHeaders, ClientIODevice::HeaderMap(), staticDirectories, m_server->nextPendingConnection(), this),
			SIGNAL(gotHeaders(HttpRequest*)),
			SLOT(announceRequest(HttpRequest*))
		);
	}

	void HttpInterface::announceRequest(HttpRequest* request)
	{
		addRequest(request);
	}

	HttpInterface::~HttpInterface()
	{
	}

	bool HttpInterface::isFinished() const
	{
		// "isFinished" means "thread load is zero; are you going to get any requests?"; answer is 'maybe'
		return false;
	}
}
