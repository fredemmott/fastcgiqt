#include "InputDevice.h"

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

		qint64 readableLength = qMin(static_cast<qint64>(m_data.length() - m_position), maxSize);
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
			::memcpy(data, m_data.constData() + m_position, readableLength);
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
