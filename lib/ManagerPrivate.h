#ifndef _FASTCGI_QT_MANAGER_PRIVATE_H
#define _FASTCGI_QT_MANAGER_PRIVATE_H

#include "Responder.h"

#include <QObject>
#include <QStringList>

class QSocketNotifier;

namespace FastCgiQt
{
	class SocketManager;
	/** @internal
	 * @brief Class managing new FastCGI connections.
	 *
	 * This class listens to the main FastCGI socket, spawns new
	 * connections, and passes them off to a SocketManager.
	 */
	class ManagerPrivate : public QObject
	{
		Q_OBJECT
		public:
			/// Create a ManagerPrivate object.
			ManagerPrivate(Responder::Generator responderGenerator, QObject* parent = NULL);
		private slots:
			/// Listen for a new FastCGI connection.
			void listen();
		private:
			/// Lock the socket with the specified socket id.
			void lockSocket(int socket);
			/// Unlock the socket with the specified socket id.
			void releaseSocket(int socket);

			/** Notifier used to watch for new connections to the
			 * FastCGI socket.
			 */
			QSocketNotifier* m_socketNotifier;
			/// Pointer to function creating new Responder objects.
			Responder::Generator m_responderGenerator;

			/** List of IP addresses that FastCGI-capable web
			 * servers may connect to this process from.
			 *
			 * @todo use this.
			 */
			QStringList m_allowedAddresses;

	};
};

#endif
