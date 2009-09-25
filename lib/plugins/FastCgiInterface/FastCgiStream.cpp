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
#include "FastCgiStream.h"

#include "EndRequestRecord.h"
#include "StandardInputRecord.h"
#include "StandardOutputRecord.h"

#include <QDebug>
#include <QLocalSocket>

#include "fastcgi.h"

namespace FastCgiQt
{
	FastCgiStream::FastCgiStream(quint16 requestId, QLocalSocket* socket)
	: QIODevice()
	, m_requestId(requestId)
	, m_socket(socket)
	{
		open(QIODevice::ReadWrite | QIODevice::Unbuffered);
		connect(m_socket, SIGNAL(readyRead()), SLOT(readSocketData()));
	}

	bool FastCgiStream::isSequential() const
	{
		return true;
	}

	void FastCgiStream::readSocketData()
	{
		// Only bother if we don't have stuff queued
		if(m_buffer.isNull())
		{
			if(!m_header.isValid())
			{
				if(m_socket->bytesAvailable() < FCGI_HEADER_LEN)
				{
					return; // Don't have a record
				}
				FCGI_Header fcgiHeader;
				const qint64 bytesRead = m_socket->read(reinterpret_cast<char*>(&fcgiHeader), FCGI_HEADER_LEN);
				if(bytesRead != FCGI_HEADER_LEN)
				{
					qFatal("Couldn't read FCGI header - tried to read %d bytes, got %lld", FCGI_HEADER_LEN, bytesRead);
				}
				m_header = RecordHeader(fcgiHeader);
				Q_ASSERT(m_header.type() == RecordHeader::StandardInputRecord);
				Q_ASSERT(m_header.requestId() == m_requestId);
			}
			if(m_socket->bytesAvailable() >= m_header.payloadLength())
			{
				const QByteArray data = m_socket->read(m_header.payloadLength());
				if(data.length() != m_header.payloadLength())
				{
					qFatal("Couldn't read payload - tried to read %d bytes, got %d", m_header.payloadLength(), data.length());
				}
				const StandardInputRecord record(m_header, data);
				m_buffer = record.streamData();
				m_header = RecordHeader();
				emit readyRead();
			}
		}
	}

	FastCgiStream::~FastCgiStream()
	{
		m_socket->write(EndRequestRecord::create(m_requestId));
		m_socket->flush();
		m_socket->close();
	}

	qint64 FastCgiStream::readData(char* data, qint64 maxSize)
	{
		Q_ASSERT(maxSize >= m_buffer.length());

	}

	qint64 FastCgiStream::writeData(const char* data, qint64 dataSize)
	{
		qint64 remaining = dataSize;
		while(remaining > 0)
		{
			const qint64 toWrite(qMin(remaining, static_cast<qint64>(65535)));
			const qint64 offset(dataSize - remaining);

			QByteArray record = StandardOutputRecord::create(
				m_requestId,
				QByteArray(&data[offset], toWrite)
			);
			qint64 wrote = m_socket->write(record);
			QLocalSocket* socket = qobject_cast<QLocalSocket*>(m_socket);
			Q_ASSERT(socket);
			if(!waitForBytesWritten(1000))
			{
				if(socket->state() != QLocalSocket::ConnectedState)
				{
					qDebug() << "NOT CONNECTED" << socket->state();
					return -1;
				}
				qFatal("Couldn't write to socket: %s %d", qPrintable(m_socket->errorString()), m_socket->isOpen());
			}
			Q_ASSERT(wrote == record.length());
			if(wrote != record.length())
			{
				qDebug() << "LENGTH MISMATCH";
				return -1;
			}
			remaining -= toWrite;
		}
		return dataSize;
	}

	bool FastCgiStream::waitForBytesWritten(int msecs)
	{
		QLocalSocket* socket = qobject_cast<QLocalSocket*>(m_socket);
		Q_ASSERT(socket);
		if(socket->state() != QLocalSocket::ConnectedState)
		{
			return false;
		}
		return m_socket->waitForBytesWritten(msecs);
	}
};
