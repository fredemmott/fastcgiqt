#include "Listener.h"

#include "RecordHeader.h"

#include "fastcgi.h"

#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QLocalSocket>
#include <QSignalMapper>

#include <errno.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace FastCgiQt
{
	Listener::Listener(QObject* parent)
		:
			QObject(parent),
			m_socket(new QLocalSocket(this)),
			m_socketMapper(new QSignalMapper(this))
	{
		connect(
			m_socketMapper,
			SIGNAL(mapped(int)),
			this,
			SLOT(processSocketData(int))
		);

		// Initialise socket address structure
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		memset(&sa, 0, len);

		// The recommended way of telling if we're running as fastcgi or not.
		int error = ::getpeername(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		if(error == -1 && errno != ENOTCONN)
		{
			qDebug() << tr("CGI not supported.");
			return;
		}

		// Listen on the socket
		qDebug() << "Accepting socket";
		lockSocket(FCGI_LISTENSOCK_FILENO);
		int newSocket = ::accept(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		releaseSocket(FCGI_LISTENSOCK_FILENO);
		::close(FCGI_LISTENSOCK_FILENO);

		// We're connected, setup a QAbstractSocket
		qDebug() << "New socket:" << newSocket;
		m_socket->setSocketDescriptor(newSocket, QLocalSocket::ConnectedState, QIODevice::ReadWrite);
		// Map readyReady to processSocketData(newSocket)
		connect(
			m_socket,
			SIGNAL(readyRead()),
			m_socketMapper,
			SLOT(map())
		);
		m_socketMapper->setMapping(m_socket, newSocket);
	}

	void Listener::lockSocket(int socket)
	{
		::flock(socket, LOCK_EX);
	}

	void Listener::releaseSocket(int socket)
	{
		::flock(socket, LOCK_UN);
	}

	void Listener::processSocketData(int socket)
	{
		Q_UNUSED(socket);
		qDebug() << "Reading data" << m_socket->bytesAvailable();
		if(m_socket->bytesAvailable() < FCGI_HEADER_LEN)
		{
			return;
		}

		FCGI_Header fcgiHeader;
		qint64 bytesRead = m_socket->read(reinterpret_cast<char*>(&fcgiHeader), FCGI_HEADER_LEN);
		if(bytesRead != FCGI_HEADER_LEN)
		{
			qFatal("Couldn't read FCGI header - tried to read %d bytes, got %d", FCGI_HEADER_LEN, bytesRead);
		}
		RecordHeader header(fcgiHeader);
		qDebug() << "Got header with record type" << header.type();
	}
}
