#include "SocketFlusher.h"

namespace FastCgiQt
{
	SocketFlusher::SocketFlusher(QAbstractSocket* socket)
	: QObject(0)
	, m_socket(socket)
	{
		socket->setParent(this);
		connect(
			socket,
			SIGNAL(bytesWritten(qint64)),
			this,
			SLOT(deleteIfFlushed())
		);
		socket->flush();
		deleteIfFlushed();
	}

	void SocketFlusher::deleteIfFlushed()
	{
		if(!m_socket->bytesToWrite())
		{
			deleteLater();
		}
	}
};
