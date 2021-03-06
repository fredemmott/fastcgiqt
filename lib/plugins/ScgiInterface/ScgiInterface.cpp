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
#include "ScgiInterface.h"
#include "ScgiRequest.h"

#include "Settings.h"

#include <QStringList>
#include <QTextStream>
#include <QTcpServer>

namespace FastCgiQt
{
	ScgiInterface::ScgiInterface(QObject* parent)
	: CommunicationInterface(parent)
	, m_tcpServer(0)
	{
	}

	QStringList ScgiInterface::backends() const
	{
		return QStringList() << "SCGI-TCP";
	}

	void ScgiInterface::spawnRequest()
	{
		addWorker(new ScgiRequest(m_tcpServer->nextPendingConnection(), NULL));
	}

	void ScgiInterface::configureHttpd(const QString& backend)
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);
		Settings settings;
		settings.beginGroup("FastCGI");

		if(backend.toUpper() == "SCGI-TCP")
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
			return;
		}
		qFatal("Unknown SCGI backend: %s", qPrintable(backend));
	}

	bool ScgiInterface::startBackend(const QString& backend)
	{
		if(backend != "SCGI-TCP")
		{
			return false;
		}
		const quint16 portNumber = Settings().value("FastCGI/portNumber", 0).value<quint16>();
		if(portNumber == 0)
		{
			return false;
		}

		m_tcpServer = new QTcpServer(this);
		if(!m_tcpServer->listen(QHostAddress::Any, portNumber))
		{
			qFatal("SCGI: Failed to listen on TCP port %d: %s", portNumber, qPrintable(m_tcpServer->errorString()));
			return false;
		}

		QTextStream cout(stdout);
		cout << "Following configuration in '" << Settings().fileName() << "' and listening for SCGI on TCP port " << portNumber << endl;

		connect(
			m_tcpServer,
			SIGNAL(newConnection()),
			SLOT(spawnRequest())
		);

		return true;
	}

	ScgiInterface::~ScgiInterface()
	{
		shutdown();
	}

	void ScgiInterface::shutdown()
	{
		if(m_tcpServer)
		{
			m_tcpServer->close();
		}
	}

	bool ScgiInterface::isFinished() const
	{
		// "isFinished" means "thread load is zero; are you going to get any requests?"; answer is 'maybe'
		return false;
	}
}
