#include "ManagerPrivate.h"

#include "BeginRequestRecord.h"
#include "EndRequestRecord.h"
#include "EnumHelpers.h"
#include "ParametersRecord.h"
#include "RecordHeader.h"
#include "StandardInputRecord.h"

#include "fastcgi.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QLocalSocket>
#include <QMetaMethod>
#include <QPointer>
#include <QSignalMapper>
#include <QSocketNotifier>
#include <QTimer>

#include <errno.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace FastCgiQt
{
	ManagerPrivate::ManagerPrivate(ResponderGenerator responderGenerator, QObject* parent)
		:
			QObject(parent),
			m_socketNotifier(new QSocketNotifier(FCGI_LISTENSOCK_FILENO, QSocketNotifier::Read, this)),
			m_responderGenerator(responderGenerator),
			m_socketMapper(new QSignalMapper(this))
	{
		connect(
			m_socketMapper,
			SIGNAL(mapped(int)),
			this,
			SLOT(processSocketData(int))
		);
		connect(
			m_socketNotifier,
			SIGNAL(activated(int)),
			this,
			SLOT(listen())
		);
		// Wait for the event loop to start up before running
		QTimer::singleShot(0, this, SLOT(listen()));
	}

	void ManagerPrivate::listen()
	{
		// Initialise socket address structure
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);

		// The recommended way of telling if we're running as fastcgi or not.
		int error = ::getpeername(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		if(error == -1 && errno != ENOTCONN)
		{
			qDebug() << tr("CGI not supported.");
			QCoreApplication::exit(-1);
			return;
		}

		// Listen on the socket
		lockSocket(FCGI_LISTENSOCK_FILENO);
		int newSocket = ::accept(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		releaseSocket(FCGI_LISTENSOCK_FILENO);

		// We're connected, setup a QLocalSocket
		QLocalSocket* socket = new QLocalSocket(this);
		socket->setSocketDescriptor(newSocket, QLocalSocket::ConnectedState, QIODevice::ReadWrite);
		// Map readyReady to processSocketData(newSocket)
		connect(
			socket,
			SIGNAL(readyRead()),
			m_socketMapper,
			SLOT(map())
		);
		m_socketMapper->setMapping(socket, newSocket);
		m_sockets.insert(newSocket, socket);
	}

	void ManagerPrivate::lockSocket(int socket)
	{
		::flock(socket, LOCK_EX);
	}

	void ManagerPrivate::releaseSocket(int socket)
	{
		::flock(socket, LOCK_UN);
	}

	void ManagerPrivate::processSocketData(int socketId)
	{
		QPointer<QLocalSocket> socket = m_sockets.value(socketId);
		bool success;
		do
		{
			if(!m_socketHeaders.contains(socketId))
			{
				success = processNewRecord(socket, socketId);
			}
			else
			{
				success = processRecordData(socket, socketId);
			}
		}
		while(success && socket && socket->bytesAvailable() > 0);
	}

	bool ManagerPrivate::processRecordData(QLocalSocket* socket, int socketId)
	{
		const RecordHeader header(m_socketHeaders.value(socketId));
		if(socket->bytesAvailable() < header.payloadLength())
		{
			return false;
		}

		m_socketHeaders.remove(socketId);

		QByteArray data = socket->read(header.payloadLength());
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
				if(!loadParameters(header, data))
				{
					respond(socket, socketId, header.requestId());
				}
				break;
			case RecordHeader::StandardInputRecord:
				readStandardInput(header, data);
				break;
			default:
				qDebug() << "Got query string data" << m_requests[header.requestId()].getData();
				qFatal("Don't know how to deal with payload for type %s", ENUM_DEBUG_STRING(RecordHeader,RecordType,header.type()));
		}
		return true;
	}

	bool ManagerPrivate::loadParameters(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::ParametersRecord);
		Q_ASSERT(m_requests.value(header.requestId()).isValid());
		ParametersRecord record(header, data);
		if(record.isEmpty())
		{
			return false;
		}
		else
		{
			m_requests[header.requestId()].addServerData(record.parameters());
			return true;
		}
	}

	void ManagerPrivate::readStandardInput(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::StandardInputRecord);
		Q_ASSERT(m_requests.value(header.requestId()).isValid());
		StandardInputRecord record(header, data);
		m_requests[header.requestId()].appendContent(record.streamData());
	}

	void ManagerPrivate::beginRequest(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::BeginRequestRecord);
		Q_ASSERT(!m_requests.value(header.requestId()).isValid());
		BeginRequestRecord record(header, *reinterpret_cast<const FCGI_BeginRequestBody*>(data.constData()));
		Q_ASSERT(record.role() == BeginRequestRecord::ResponderRole);

		if(m_requests.size() < record.requestId())
		{
			m_requests.resize((record.requestId() + 1) * 2);
		}
		if(m_closeSocketOnExit.size() < record.requestId())
		{
			m_closeSocketOnExit.resize((record.requestId() + 1) * 2);
		}
		m_requests[record.requestId()] = Request(record.requestId());
		m_closeSocketOnExit[record.requestId()] = ! (record.flags() & BeginRequestRecord::KeepConnection);
	}

	void ManagerPrivate::queueSocketCheck(int socket)
	{
		staticMetaObject.invokeMethod(this, "processSocketData", Qt::QueuedConnection, Q_ARG(int, socket));
	}

	void ManagerPrivate::respond(QLocalSocket* socket, int socketId, quint16 requestId)
	{
		Responder* responder = (*m_responderGenerator)(
			m_requests.at(requestId),
			socket,
			this
		);
		// in case we have more local data...
		queueSocketCheck(socketId);
		
		// actually start the response
		responder->respond();

		// clean up any pending events
		QCoreApplication::processEvents();

		// clean up the responder
		delete responder; // clean up IO devices before cleaning up socket.

		// cleanup the socket
		socket->write(EndRequestRecord::create(requestId));
		socket->flush();
		// close and delete if the server's lazy
		if(m_closeSocketOnExit.value(requestId))
		{
			socket->close();
			m_sockets.remove(socketId);
			delete socket;
		}
		m_requests[requestId] = Request();
	}

	bool ManagerPrivate::processNewRecord(QLocalSocket* socket, int socketId)
	{
		if(socket->bytesAvailable() < FCGI_HEADER_LEN)
		{
			return false;
		}

		FCGI_Header fcgiHeader;
		qint64 bytesRead = socket->read(reinterpret_cast<char*>(&fcgiHeader), FCGI_HEADER_LEN);
		if(bytesRead != FCGI_HEADER_LEN)
		{
			qFatal("Couldn't read FCGI header - tried to read %d bytes, got %d", FCGI_HEADER_LEN, bytesRead);
		}
		RecordHeader header(fcgiHeader);
		m_socketHeaders.insert(socketId, header);
		if(socket->bytesAvailable() >= header.payloadLength())
		{
			processRecordData(socket, socketId);
		}
		return true;
	}
}
