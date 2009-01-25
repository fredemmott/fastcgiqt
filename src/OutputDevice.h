#ifndef _FASTCGI_QT_OUTPUT_DEVICE_H
#define _FASTCGI_QT_OUTPUT_DEVICE_H

#include <QIODevice>

namespace FastCgiQt
{
	class OutputDevice : public QIODevice
	{
		Q_OBJECT
		public:
			OutputDevice(quint16 requestId, QIODevice* socket, QObject* parent = NULL);
			qint64 readData(char* data, qint64 maxSize);
			qint64 writeData(const char* data, qint64 maxSize);
		private:
			quint16 m_requestId;
			QIODevice* m_socket;
	};
}

#endif
