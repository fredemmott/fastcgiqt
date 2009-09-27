/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include "FastCgiInterface.h"

#include "DebugHandler.h"
#include "Settings.h"
#include "FastCgiSocketManager.h"

#include "fastcgi.h"

#include <QtEndian>
#include <QFileSystemWatcher>
#include <QHostAddress>
#include <QSocketNotifier>
#include <QTextStream>
#include <QTimer>

#include <errno.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/ip.h>

namespace FastCgiQt
{
	FastCgiInterface::FastCgiInterface(Responder::Generator generator, QObject* parent)
	: CommunicationInterface(generator, parent)
	, m_socketNotifier(0)
	{
	}

	QStringList FastCgiInterface::backends() const
	{
		return QStringList() << "FCGI-UNIX" << "FCGI-TCP";
	}

	void FastCgiInterface::configureHttpd(const QString& backend)
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);
		Settings settings;
		settings.beginGroup("FastCGI");

		if(backend.toUpper() == "FCGI-UNIX")
		{
			return;
		}
		if(backend.toUpper() == "FCGI-TCP")
		{
			QString portString;
			cout << "Port number: " << flush;
			portString = cin.readLine();
			bool ok;
			quint32 portNumber = portString.toUInt(&ok);
			if(!(ok && portNumber))
			{
				qFatal("Not a valid port number.");
				return;
			}
			settings.setValue("portNumber", portNumber);
			return;
		}
		qFatal("Unknown FastCGI backend: %s", qPrintable(backend));
	}

	bool FastCgiInterface::startBackend(const QString& backend)
	{
		if(backend == "FCGI-UNIX")
		{
			new DebugHandler(this);
		}
		// Check we're running as a FastCGI application
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);
		m_socket = FCGI_LISTENSOCK_FILENO;

		// The recommended way of telling if we're running as fastcgi or not.
		int error = ::getpeername(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		if(error == -1 && errno != ENOTCONN)
		{
			Settings settings;
			if(backend == "FCGI-TCP")
			{
				m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
				in_port_t port = settings.value("FastCGI/portNumber", 0).value<in_port_t>();
				if(port == 0)
				{
					qFatal("Configured to listen on TCP, but there isn't a valid Port Number configured. Try --configure-fastcgi");
					return false;
				}
				sockaddr_in sa;
				::memset(&sa, 0, sizeof(sa));
				sa.sin_family = AF_INET;
				sa.sin_port = qToBigEndian(port);
				if(::bind(m_socket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa)) == -1)
				{
					qFatal("Failed to bind() to TCP port %d, with error %s", port, ::strerror(errno));
					return false;
				}
				if(::listen(m_socket, 1) == -1)
				{
					qFatal("Failed to listen() on port %d, with error %s", port, ::strerror(errno));
					return false;
				}
				QTextStream cout(stdout);
				cout << "Following configuration in '" << settings.fileName() << "' and listening for FastCGI on TCP port " << port << endl;
			}
			else
			{
				return false;
			}
		}

		m_socketNotifier = new QSocketNotifier(m_socket, QSocketNotifier::Read, this),

		connect(
			m_socketNotifier,
			SIGNAL(activated(int)),
			this,
			SLOT(listen())
		);

		// Wait for the event loop to start up before running
		QTimer::singleShot(0, this, SLOT(listen()));

		return true;
	}

	FastCgiInterface::~FastCgiInterface()
	{
		shutdown();
	}

	void FastCgiInterface::shutdown()
	{
		if(m_socketNotifier)
		{
			// stop listening on the main socket
			::close(m_socketNotifier->socket());
			// stop watching it
			m_socketNotifier->setEnabled(false);
		}
	}

	bool FastCgiInterface::isFinished() const
	{
		return !m_socketNotifier->isEnabled();
	}

	void FastCgiInterface::listen()
	{
		// Initialise socket address structure
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);

		// Listen on the socket
		lockSocket(m_socket);
		int newSocket = ::accept(m_socket, reinterpret_cast<sockaddr*>(&sa), &len);
		releaseSocket(m_socket);

		/* We're connected, setup a FastCgiSocketManager.
		 * This will delete itself when appropriate (via deleteLater())
		 */
		FastCgiSocketManager* socket = new FastCgiSocketManager(newSocket, NULL);
		addWorker(socket);
	}

	void FastCgiInterface::lockSocket(int socket)
	{
		::flock(socket, LOCK_EX);
	}

	void FastCgiInterface::releaseSocket(int socket)
	{
		::flock(socket, LOCK_UN);
	}
}
