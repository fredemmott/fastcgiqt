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
#include "OutputDevice.h"

#include "StandardOutputRecord.h"

namespace FastCgiQt
{
	OutputDevice::OutputDevice(quint16 requestId, QIODevice* socket, QObject* parent)
		:
			QIODevice(parent),
			m_haveSentData(false),
			m_requestId(requestId),
			m_socket(socket),
			m_mode(Streamed),
			m_sendHeaders(true)
	{
		open(QIODevice::WriteOnly);
		m_headers.insert("CONTENT-TYPE", "text/html; charset=UTF-8");
		m_headers.insert("STATUS", "200 OK");
	}

	bool OutputDevice::isSendingHeadersEnabled() const
	{
		return m_sendHeaders;
	}

	void OutputDevice::setSendingHeadersEnabled(bool enabled)
	{
		Q_ASSERT(!haveSentData());
		m_sendHeaders = enabled;
	}

	void OutputDevice::setMode(Mode mode)
	{
		Q_ASSERT(!haveSentData());
		Q_ASSERT(mode != Buffered); // not yet supported
		m_mode = mode;
	}

	OutputDevice::Mode OutputDevice::mode() const
	{
		return m_mode;
	}

	QByteArray OutputDevice::buffer() const
	{
		return m_buffer;
	}

	qint64 OutputDevice::readData(char* data, qint64 maxSize)
	{
		Q_UNUSED(data);
		Q_UNUSED(maxSize);
		return -1;
	}

	bool OutputDevice::haveSentData() const
	{
		return m_haveSentData;
	}

	qint64 OutputDevice::writeData(const char* data, qint64 maxSize)
	{
		if(isSendingHeadersEnabled() && !m_haveSentData)
		{
			m_haveSentData = true;
			QString headerData;
			for(
				QHash<QString, QString>::ConstIterator it = m_headers.constBegin();
				it != m_headers.constEnd();
				++it
			)
			{
				headerData += QString("%1: %2\r\n").arg(it.key(), it.value());
			}
			headerData += "\r\n";
			QByteArray raw = headerData.toUtf8();
			writeData(raw.constData(), raw.length());
		}

		QByteArray record = StandardOutputRecord::create(
			m_requestId,
			QByteArray::fromRawData(data, maxSize)
		);
		qint64 wrote = m_socket->write(record.constData(), record.length());
		Q_ASSERT(wrote == record.length());
		if(wrote == record.length())
		{
			if(mode() == Logged)
			{
				m_buffer.append(QByteArray(data, maxSize));
			}
			return maxSize;
		}
		else
		{
			return -1;
		}
	}

	bool OutputDevice::setHeader(const QString& name, const QString& value)
	{
		if(m_haveSentData)
		{
			qCritical("Attempted to set a header after data has been sent.");
			return false;
		}

		m_headers[name.toUpper()] = value;
		return true;
	}
}
