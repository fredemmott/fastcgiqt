#include "SocketServer.h"

#include <QSocketNotifier>

namespace FastCgiQt
{
	class SocketServer::Private
	{
		public:
			/// Lock the socket with the specified socket id.
			void lockSocket(int socket);
			/// Unlock the socket with the specified socket id.
			void releaseSocket(int socket);

			/// Socket handle
			int m_socket;

			/** Notifier used to watch for new connections to the
			 * FastCGI socket.
			 */
			QSocketNotifier* m_socketNotifier;
	};

	SocketServer::SocketTypes SocketServer::supportedSocketTypes()
	{
		return TcpSocket | UnixSocket;
	}

	SocketServer::SocketServer(QObject* parent)
	: QObject(parent)
	, d(new SocketServer::Private)
	{
	}

	SocketServer::~SocketServer()
	{
		delete d;
	}
};
