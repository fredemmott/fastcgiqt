#include "Manager.h"

#include "BeginRequestRecord.h"
#include "EnumHelpers.h"
#include "ParametersRecord.h"
#include "RecordHeader.h"
#include "StandardInputRecord.h"

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
	Manager::Manager(QObject* parent)
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

	void Manager::lockSocket(int socket)
	{
		::flock(socket, LOCK_EX);
	}

	void Manager::releaseSocket(int socket)
	{
		::flock(socket, LOCK_UN);
	}

	void Manager::processSocketData(int socket)
	{
		bool success;
		do
		{
			if(!m_socketHeaders.contains(socket))
			{
				success = processNewRecord(socket);
			}
			else
			{
				success = processRecordData(socket);
			}
		}
		while(success && m_socket->bytesAvailable() > 0);
	}

	bool Manager::processRecordData(int socket)
	{
		qDebug() << "Payload on socket" << socket;
		const RecordHeader header(m_socketHeaders.value(socket));
		if(m_socket->bytesAvailable() < header.payloadLength())
		{
			return false;
		}

		QByteArray data = m_socket->read(header.payloadLength());
		qint64 bytesRead = data.length();

		if(bytesRead != header.payloadLength())
		{
			qFatal("Couldn't read payload - tried to read %d bytes, got %d", header.payloadLength(), bytesRead);
		}
		switch(header.type())
		{
			case RecordHeader::BeginRequestRecord:
				beginRequest(header, data);
				break;
			case RecordHeader::ParametersRecord:
				loadParameters(header, data);
				break;
			case RecordHeader::StandardInputRecord:
				readStandardInput(header, data);
				break;
			default:
				qDebug() << "Got query string data" << m_requests[header.requestId()].getData();
				qFatal("Don't know how to deal with payload for type %s", ENUM_DEBUG_STRING(RecordHeader,RecordType,header.type()));
		}
		m_socketHeaders.remove(socket);
		return true;
	}

	void Manager::loadParameters(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::ParametersRecord);
		Q_ASSERT(m_requests.value(header.requestId()).isValid());
		ParametersRecord record(header, data);
		m_requests[header.requestId()].addServerData(record.parameters());
	}

	void Manager::readStandardInput(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::StandardInputRecord);
		Q_ASSERT(m_requests.value(header.requestId()).isValid());
		StandardInputRecord record(header, data);
		m_requests[header.requestId()].appendContent(record.streamData());
		qDebug() << "Read standard input - done?" << m_requests.value(header.requestId()).haveContent();
	}

	void Manager::beginRequest(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::BeginRequestRecord);
		Q_ASSERT(!m_requests.value(header.requestId()).isValid());
		BeginRequestRecord record(header, *reinterpret_cast<const FCGI_BeginRequestBody*>(data.constData()));
		qDebug() << "Got new begin request with id" << record.requestId() << "role" << record.role() << "flags" << record.flags();
		Q_ASSERT(record.role() == BeginRequestRecord::ResponderRole);

		if(m_requests.size() -1 < record.requestId())
		{
			m_requests.resize((record.requestId() + 1) + 2);
			m_requests[record.requestId()] = Request(record.requestId());
		}
	}

	bool Manager::processNewRecord(int socket)
	{
		qDebug() << "New record on socket" << socket;
		if(m_socket->bytesAvailable() < FCGI_HEADER_LEN)
		{
			return false;
		}

		FCGI_Header fcgiHeader;
		qint64 bytesRead = m_socket->read(reinterpret_cast<char*>(&fcgiHeader), FCGI_HEADER_LEN);
		if(bytesRead != FCGI_HEADER_LEN)
		{
			qFatal("Couldn't read FCGI header - tried to read %d bytes, got %d", FCGI_HEADER_LEN, bytesRead);
		}
		RecordHeader header(fcgiHeader);
		qDebug() << "Got header with record type" << header.type() << "id" << header.requestId() << "payload size" << header.payloadLength() << "content size" << header.contentLength();
		m_socketHeaders.insert(socket, header);
		if(m_socket->bytesAvailable() >= header.payloadLength())
		{
			processRecordData(socket);
		}
		return true;
	}
}
