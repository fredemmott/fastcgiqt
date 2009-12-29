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
#include "SocketServer.h"

#include "fastcgi.h"

#ifdef WITH_SYSLOG_SUPPORT
#include "DebugHandler.h"
#endif

namespace FastCgiQt
{
	FastCgiInterface::FastCgiInterface(QObject* parent)
	: CommunicationInterface(parent)
	, m_socketServer(new SocketServer(this))
	{
		connect(
			m_socketServer,
			SIGNAL(newConnection()),
			this,
			SLOT(acceptNewConnection())
		);
	}

	QStringList FastCgiInterface::backends() const
	{
		QStringList stringOutput;
		SocketServer::SocketTypes supported = SocketServer::supportedSocketTypes();
		if(supported.testFlag(SocketServer::UnixSocket)) stringOutput << "FCGI-UNIX";
		if(supported.testFlag(SocketServer::TcpSocket)) stringOutput << "FCGI-TCP";
		return stringOutput;
	}

	void FastCgiInterface::configureHttpd(const QString& backend)
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);
		Settings settings;
		settings.beginGroup("FastCGI");

		if(backend.toUpper() == "FCGI-UNIX")
		{
			return;
		}
		if(backend.toUpper() == "FCGI-TCP")
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
		qFatal("Unknown FastCGI backend: %s", qPrintable(backend));
	}

	bool FastCgiInterface::startBackend(const QString& backend)
	{
		quint16 port = 0;
		SocketServer::SocketType socketType;

		if(backend == "FCGI-UNIX")
		{
			socketType = SocketServer::UnixSocket;
#ifdef WITH_SYSLOG_SUPPORT
			new DebugHandler(this);
#endif
		}

		Settings settings;
		if(backend == "FCGI-TCP")
		{
			port = settings.value("FastCGI/portNumber", 0).value<quint16>();
			QTextStream cout(stdout);
			socketType = SocketServer::TcpSocket;

			cout << "Following configuration in '" << settings.fileName() << "' and listening for FastCGI on TCP port " << port << endl;
		}

		return m_socketServer->listen(socketType, port);
	}

	FastCgiInterface::~FastCgiInterface()
	{
	}

	bool FastCgiInterface::isFinished() const
	{
		return m_socketServer->isFinished();
	}

	void FastCgiInterface::acceptNewConnection()
	{
		/* We're connected, setup a FastCgiSocketManager.
		 * This will delete itself when appropriate (via deleteLater())
		 */
		FastCgiSocketManager* socket = new FastCgiSocketManager(m_socketServer->nextPendingConnection(), 0);
		addWorker(socket);
	}
}
