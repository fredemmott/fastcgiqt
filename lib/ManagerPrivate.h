#ifndef _FASTCGI_QT_MANAGER_PRIVATE_H
#define _FASTCGI_QT_MANAGER_PRIVATE_H

#include "Responder.h"

#include <QObject>
#include <QStringList>

class QSocketNotifier;

namespace FastCgiQt
{
	class SocketManager;
	class ManagerPrivate : public QObject
	{
		Q_OBJECT
		public:
			ManagerPrivate(Responder::Generator responderGenerator, QObject* parent = NULL);
		private slots:
			void listen();
		private:
			void lockSocket(int socket);
			void releaseSocket(int socket);

			QSocketNotifier* m_socketNotifier;
			Responder::Generator m_responderGenerator;

			QStringList m_allowedAddresses;

	};
};

#endif
