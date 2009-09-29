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
#include "InputDevice.h"

#include "memcpy_safe.h"

#include <QCoreApplication>
#include <QTime>

namespace FastCgiQt
{
	InputDevice::InputDevice(QObject* parent)
		:
			QIODevice(parent),
			m_position(NULL),
			m_atEnd(false)
	{
		open(QIODevice::ReadOnly);
	}

	qint64 InputDevice::readData(char* data, qint64 maxSize)
	{
		if(maxSize < 0)
		{
			return -1;
		}

		const qint64 readableLength = qMin(static_cast<qint64>(m_data.length() - m_position), maxSize);
		Q_ASSERT(readableLength >= 0);
		if(readableLength == 0)
		{
			if(!m_atEnd)
			{
				waitForReadyRead(-1);
				return readData(data, maxSize);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			// we can read some stuff.
			::memcpy_safe(data, maxSize, m_data.constData() + m_position, readableLength);
			m_position += readableLength;
			return readableLength;
		}
	}

	bool InputDevice::waitForReadyRead(int msecs)
	{
		const qint64 length(m_data.length());
		QTime time;
		time.start();
		while(
			(!m_atEnd) // still data left to arrive
			&&
			(length == m_data.length())
			&&
			(
				(msecs < 0) // forever
				||
				(msecs > time.elapsed())
			)
		)
		{
			QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
		}
		return length != m_data.length();
	}

	qint64 InputDevice::writeData(const char* data, qint64 maxSize)
	{
		Q_UNUSED(data);
		Q_UNUSED(maxSize);
		return -1;
	}

	void InputDevice::appendData(const QByteArray& data)
	{
		if(data.isEmpty())
		{
			m_atEnd = true;
		}
		else
		{
			m_data.append(data);
			emit readyRead();
		}
	}
}
