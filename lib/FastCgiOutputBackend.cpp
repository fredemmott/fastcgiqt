#include "FastCgiOutputBackend.h"

#include "StandardOutputRecord.h"

#include <QDebug>
#include <QLocalSocket>

namespace FastCgiQt
{
	FastCgiOutputBackend::FastCgiOutputBackend(quint16 requestId, QIODevice* socket)
	: Backend()
	, m_requestId(requestId)
	, m_socket(socket)
	{
		open(QIODevice::WriteOnly | QIODevice::Unbuffered);
	}

	qint64 FastCgiOutputBackend::writeData(const char* data, qint64 dataSize)
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

	bool FastCgiOutputBackend::waitForBytesWritten(int msecs)
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
