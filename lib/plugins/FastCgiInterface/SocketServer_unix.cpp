/** Original implementation supporting both UNIX and TCP sockets.
 *
 * The UNIX socket implementation is based on the behaviour defined in the FastCGI specification, and also
 * includes optional support for logging via syslog, as defined in the specification.
 *
 * This implementation is *NOT* portable.
 */
#include "SocketServer.h"

#include "fastcgi.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QHostAddress>
#include <QSocketNotifier>
#include <QTextStream>
#include <QtEndian>

#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

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

			/// Type of socket
			SocketServer::SocketType m_socketType;

			/** Notifier used to watch for new connections to the
			 * FastCGI socket.
			 */
			QSocketNotifier* m_socketNotifier;
	};

	SocketServer::SocketTypes SocketServer::activeSocketTypes()
	{
		// Check we're running as a FastCGI application
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);

		// The recommended way of telling if we're running as fastcgi or not.
		const int error = ::getpeername(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		if(error == -1 && errno != ENOTCONN)
		{
			// Nope, not FastCGI over unix socket
			return SocketTypes();
		}
		return UnixSocket;
	}

	bool SocketServer::isFinished() const
	{
		return !(d->m_socketNotifier && d->m_socketNotifier->isEnabled());
	}

	void SocketServer::Private::lockSocket(int socket)
	{
#ifdef WITHOUT_FLOCK
		struct flock fl;
		::bzero(&fl, sizeof(fl));

		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;

		::fcntl(socket, F_SETLKW, &fl);
#else
		::flock(socket, LOCK_EX);
#endif
	}

	void SocketServer::Private::releaseSocket(int socket)
	{
#ifdef WITHOUT_FLOCK
		struct flock fl;
		::bzero(&fl, sizeof(fl));

		fl.l_type = F_UNLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;

		::fcntl(socket, F_SETLK, &fl);
#else
		::flock(socket, LOCK_UN);
#endif
	}

	bool SocketServer::listen(SocketType type, quint16 parameter)
	{
		d->m_socketType = type;
		if(type == UnixSocket)
		{
			if(! (activeSocketTypes() & UnixSocket))
			{
				qWarning("Asked to run FastCGI on a UNIX socket, but not being run correctly");
				return false;
			}
			d->m_socket = FCGI_LISTENSOCK_FILENO;
		}
		else
		{
			Q_ASSERT(type == TcpSocket);
			// SOCK_STREAM specifies TCP instead of UDP
			d->m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
			const in_port_t port = static_cast<in_port_t>(parameter);
			if(port == 0)
			{
					qWarning("Configured to listen on TCP, but there isn't a valid Port Number configured. Try --configure-fastcgi");
					return false;
			}

			// Initialise the socket address structure
			sockaddr_in sa;
			::memset(&sa, 0, sizeof(sa));
			sa.sin_family = AF_INET;
			sa.sin_port = qToBigEndian(port);

			int one = 1;
			if(setsockopt(d->m_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == -1)
			{
				qWarning("Failed to set SO_REUSEADDR option - error: %s", ::strerror(errno));
			}
			// Try and bind to the port
			if(::bind(d->m_socket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) == -1)
			{
				qWarning("Failed to bind() to TCP port %d with error: %s", port, ::strerror(errno));
				return false;
			}
			// Now we've bound to the port, start listening
			if(::listen(d->m_socket, 1) == -1)
			{
				qWarning("Failed to listen() on port %d with error: %s", port, ::strerror(errno));
				return false;
			}
		}
		
		d->m_socketNotifier = new QSocketNotifier(d->m_socket, QSocketNotifier::Read, this),
		connect(
			d->m_socketNotifier,
			SIGNAL(activated(int)),
			this,
			SIGNAL(newConnection())
		);

		return true;
	}

	SocketServer::SocketType SocketServer::socketType() const
	{
		return d->m_socketType;
	}

	QTcpSocket* SocketServer::nextPendingConnection()
	{
		// Initialise socket address structure
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);

		// Listen on the socket
		d->lockSocket(d->m_socket);
		const int newSocket = ::accept(d->m_socket, reinterpret_cast<sockaddr*>(&sa), &len);
		d->releaseSocket(d->m_socket);

		if(newSocket == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
		{
			return 0;
		}
		else
		{
			QTcpSocket* socket = new QTcpSocket(this);
			socket->setSocketDescriptor(newSocket);
			return socket;
		}
	}

	SocketServer::SocketTypes SocketServer::supportedSocketTypes()
	{
		return TcpSocket | UnixSocket;
	}

	SocketServer::SocketServer(QObject* parent)
	: QObject(parent)
	, d(new SocketServer::Private)
	{
		d->m_socketNotifier = 0;
	}

	SocketServer::~SocketServer()
	{
		if(d->m_socketNotifier)
		{
			Q_ASSERT(d->m_socket == d->m_socketNotifier->socket());
			// stop listening
			::close(d->m_socket);
			// stop watching
			d->m_socketNotifier->setEnabled(false);
		}
		delete d;
	}
};
