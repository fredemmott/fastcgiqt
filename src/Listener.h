#ifndef _FASTCGI_QT_LISTENER_H
#define _FASTCGI_QT_LISTENER_H

#include <QObject>
#include <QStringList>

class QLocalSocket;
class QSignalMapper;

namespace FastCgiQt
{
	class Listener : public QObject
	{
		Q_OBJECT
		public:
			Listener(QObject* parent = NULL);
		private slots:
			void processSocketData(int socket);
		private:
			void lockSocket(int socket);
			void releaseSocket(int socket);
			QLocalSocket* m_socket;

			/// Used to map readyRead signals to processSocketData(int socket)
			QSignalMapper* m_socketMapper;
			QStringList m_allowedAddresses;
	};
};

#endif
