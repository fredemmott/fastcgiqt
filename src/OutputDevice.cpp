#include "OutputDevice.h"

#include "StandardOutputRecord.h"

namespace FastCgiQt
{
	OutputDevice::OutputDevice(quint16 requestId, QIODevice* socket, QObject* parent)
		:
			QIODevice(parent),
			m_requestId(requestId),
			m_socket(socket)
	{
	}

	qint64 OutputDevice::readData(char* data, qint64 maxSize)
	{
		Q_UNUSED(data);
		Q_UNUSED(maxSize);
		return -1;
	}

	qint64 OutputDevice::writeData(const char* data, qint64 maxSize)
	{
		QByteArray record = StandardOutputRecord::create(
			m_requestId,
			QByteArray::fromRawData(data, maxSize)
		);
		qint64 wrote = m_socket->write(record.constData(), record.length());
		Q_ASSERT(wrote == record.length());
		if(wrote == record.length())
		{
			return maxSize;
		}
		else
		{
			return -1;
		}
	}
}
