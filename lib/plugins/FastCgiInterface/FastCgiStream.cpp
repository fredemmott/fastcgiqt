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

#include "memcpy_safe.h"

#include <QDebug>
#include <QLocalSocket>

#include "fastcgi.h"


namespace FastCgiQt
{
	FastCgiStream::FastCgiStream(const HeaderMap& headers, quint16 requestId, QLocalSocket* socket, QObject* parent)
	: ClientIODevice(parent)
	, m_requestHeaders(headers)
	, m_requestBufferReadPosition(0)
	, m_requestId(requestId)
	, m_socket(socket)
	{
		open(QIODevice::ReadWrite | QIODevice::Unbuffered);
		connect(
			m_socket,
			SIGNAL(aboutToClose()),
			SIGNAL(aboutToClose())
		);
	}

	ClientIODevice::HeaderMap FastCgiStream::requestHeaders() const
	{
		return m_requestHeaders;
	}

	qint64 FastCgiStream::bytesAvailable() const
	{
		return m_requestBuffer.length() - m_requestBufferReadPosition;
	}

	void FastCgiStream::appendData(const QByteArray& data)
	{
		m_requestBuffer.append(data);
		emit readyRead();
	}

	FastCgiStream::~FastCgiStream()
	{
		m_socket->write(EndRequestRecord::create(m_requestId));
		m_socket->flush();
		m_socket->close(); // TODO: check the flag; but every httpd sets it anyway...
	}

	qint64 FastCgiStream::readData(char* data, qint64 maxSize)
	{
		const qint64 toRead = qMin(m_requestBuffer.length() - m_requestBufferReadPosition, maxSize);
		if(toRead >= 0 && ::memcpy_safe(data, maxSize, m_requestBuffer.constData(), toRead))
		{
			m_requestBufferReadPosition += toRead;
			return toRead;
		}
		else
		{
			return -1;
		}
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
