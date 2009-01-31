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

#include "BeginRequestRecord.h"
#include "EndRequestRecord.h"
#include "EnumHelpers.h"
#include "InputDevice.h"
#include "ParametersRecord.h"
#include "RecordHeader.h"
#include "RequestDataProvider.h"
#include "SocketManager.h"
#include "StandardInputRecord.h"

#include "fastcgi.h"

#include <QCoreApplication>
#include <QDebug>
#include <QLocalSocket>
#include <QSocketNotifier>
#include <QTimer>

namespace FastCgiQt
{
	SocketManager::SocketManager(Responder::Generator responderGenerator, int socketId, QObject* parent)
		:
			QObject(parent),
			m_responderGenerator(responderGenerator)
	{
		// We're connected, setup a QLocalSocket
		m_socket = new QLocalSocket(this);
		m_socket->setSocketDescriptor(socketId , QLocalSocket::ConnectedState, QIODevice::ReadWrite);

		connect(
			m_socket,
			SIGNAL(readyRead()),
			this,
			SLOT(processSocketData())
		);
	}

	SocketManager::~SocketManager()
	{
	}

	void SocketManager::processSocketData()
	{
		bool success;
		do
		{
			if(!m_recordHeader.isValid())
			{
				success = processNewRecord();
			}
			else
			{
				success = processRecordData();
			}
		}
		while(success && m_socket->bytesAvailable() > 0);
	}

	bool SocketManager::processRecordData()
	{
		RecordHeader header(m_recordHeader);

		if(m_socket->bytesAvailable() < header.payloadLength())
		{
			return false;
		}

		QByteArray data = m_socket->read(header.payloadLength());
		qint64 bytesRead = data.length();

		if(bytesRead != header.payloadLength())
		{
			qFatal("Couldn't read payload - tried to read %d bytes, got %lld", header.payloadLength(), bytesRead);
		}

		switch(header.type())
		{
			case RecordHeader::BeginRequestRecord:
				beginRequest(data);
				break;
			case RecordHeader::ParametersRecord:
				if(!loadParameters(data))
				{
					respond();
				}
				break;
			case RecordHeader::StandardInputRecord:
				readStandardInput(data);
				break;
			default:
				qDebug() << "Got query string data" << m_requests[header.requestId()].getData();
				qFatal("Don't know how to deal with payload for type %s", ENUM_DEBUG_STRING(RecordHeader,RecordType,header.type()));
		}
		m_recordHeader = RecordHeader();
		return true;
	}

	bool SocketManager::loadParameters(const QByteArray& data)
	{
		quint16 requestId = m_recordHeader.requestId();
		Q_ASSERT(m_recordHeader.type() == RecordHeader::ParametersRecord);
		Q_ASSERT(m_requests.value(requestId).isValid());
		ParametersRecord record(m_recordHeader, data);
		if(record.isEmpty())
		{
			return false;
		}
		else
		{
			RequestDataProvider::addServerData(&m_requests[requestId], record.parameters());
			return true;
		}
	}

	void SocketManager::readStandardInput(const QByteArray& data)
	{
		quint16 requestId = m_recordHeader.requestId();
		Q_ASSERT(m_recordHeader.type() == RecordHeader::StandardInputRecord);
		Q_ASSERT(m_requests.value(requestId).isValid());
		StandardInputRecord record(m_recordHeader, data);
		RequestDataProvider::appendContent(&m_requests[requestId], record.streamData());
		m_inputDevices[requestId]->appendData(record.streamData());
	}

	void SocketManager::beginRequest(const QByteArray& data)
	{
		quint16 requestId = m_recordHeader.requestId();
		Q_ASSERT(m_recordHeader.type() == RecordHeader::BeginRequestRecord);
		Q_ASSERT(!m_requests.value(requestId).isValid());

		BeginRequestRecord record(m_recordHeader, *reinterpret_cast<const FCGI_BeginRequestBody*>(data.constData()));
		Q_ASSERT(record.role() == BeginRequestRecord::ResponderRole);

		if(m_requests.size() <= requestId)
		{
			m_requests.resize((requestId + 1) * 2);
		}
		if(m_closeSocketOnExit.size() <= requestId)
		{
			m_closeSocketOnExit.resize((requestId + 1) * 2);
		}
		if(m_inputDevices.size() <= requestId)
		{
			m_inputDevices.resize((requestId + 1) * 2);
		}
		m_requests[requestId] = Request(requestId);
		m_closeSocketOnExit[requestId] = ! (record.flags() & BeginRequestRecord::KeepConnection);
		m_inputDevices[requestId] = new InputDevice(this);
	}

	void SocketManager::queueSocketCheck()
	{
		QTimer::singleShot(0, this, SLOT(processSocketData()));
	}

	void SocketManager::respond()
	{
		quint16 requestId = m_recordHeader.requestId();
		Responder* responder = (*m_responderGenerator)(
			m_requests.at(requestId),
			m_socket,
			m_inputDevices.at(requestId),
			this
		);
		// in case we have more local data...
		m_recordHeader = RecordHeader();
		queueSocketCheck();
		
		// actually start the response
		responder->respond();

		// clean up any pending events
		QCoreApplication::processEvents();

		// clean up the responder
		delete responder; // clean up IO devices before cleaning up socket.

		// cleanup the socket
		m_socket->write(EndRequestRecord::create(requestId));
		m_socket->flush();
		// close and delete if the server's lazy
		if(m_closeSocketOnExit.value(requestId))
		{
			m_socket->close();
			deleteLater();
		}
		m_requests[requestId] = Request();
		delete m_inputDevices[requestId];
		m_inputDevices[requestId] = 0;
	}

	bool SocketManager::processNewRecord()
	{
		if(m_socket->bytesAvailable() < FCGI_HEADER_LEN)
		{
			return false;
		}

		FCGI_Header fcgiHeader;
		qint64 bytesRead = m_socket->read(reinterpret_cast<char*>(&fcgiHeader), FCGI_HEADER_LEN);
		if(bytesRead != FCGI_HEADER_LEN)
		{
			qFatal("Couldn't read FCGI header - tried to read %d bytes, got %lld", FCGI_HEADER_LEN, bytesRead);
		}
		m_recordHeader = RecordHeader(fcgiHeader);
		if(m_socket->bytesAvailable() >= m_recordHeader.payloadLength())
		{
			processRecordData();
		}
		return true;
	}
}
