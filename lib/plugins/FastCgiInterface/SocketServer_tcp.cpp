/* Pure-Qt TCP-only implementation.
 *
 * This is meant to be portable to Windows.
 */
#include "SocketServer.h"

#include <QTcpServer>

namespace FastCgiQt
{
	class SocketServer::Private
	{
		public:
			QTcpServer* m_tcpServer;
	};

	SocketServer::SocketTypes SocketServer::supportedSocketTypes()
	{
		return TcpSocket;
	}

	SocketServer::SocketTypes SocketServer::activeSocketTypes()
	{
		return SocketTypes();
	}

	SocketServer::SocketServer(QObject* parent)
	: QObject(parent)
	, d(new Private())
	{
		d->m_tcpServer = new QTcpServer(this);
		connect(
			d->m_tcpServer,
			SIGNAL(newConnection()),
			this,
			SIGNAL(newConnection())
		);
	}

	SocketServer::SocketType SocketServer::socketType() const
	{
		return TcpSocket;
	}

	SocketServer::~SocketServer()
	{
		delete d;
	}

	bool SocketServer::isFinished() const
	{
		// With the TCP model, there's no concept of a server-side "we're done"
		return false;
	}

	bool SocketServer::listen(SocketType type, quint16 portNumber)
	{
		Q_ASSERT(type == TcpSocket);
		Q_ASSERT(portnumber != 0);
		if(type != TcpSocket || portNumber == 0)
		{
			return false;
		}
		return d->m_tcpServer->listen(QHostAddress::Any, portNumber);
	}

	QTcpSocket* SocketServer::nextPendingConnection()
	{
		return d->m_tcpServer->nextPendingConnection();
	}
}
