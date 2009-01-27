#ifndef _FASTCGI_QT_INPUT_DEVICE_H
#define _FASTCGI_QT_INPUT_DEVICE_H

#include <QIODevice>

namespace FastCgiQt
{
	class InputDevice : public QIODevice
	{
		Q_OBJECT
		public:
			InputDevice(QObject* parent = NULL);
			qint64 readData(char* data, qint64 maxSize);
			qint64 writeData(const char* data, qint64 maxSize);

			bool waitForReadyRead(int msecs);

			void appendData(const QByteArray& data);
		private:
			quint64 m_position;
			bool m_atEnd;
			QByteArray m_data;
	};
}

#endif
