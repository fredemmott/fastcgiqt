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
#include "ManagerPrivate.h"

#include "SocketManager.h"

#include "fastcgi.h"

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QSocketNotifier>
#include <QThread>
#include <QTime>
#include <QTimer>

#include <errno.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace FastCgiQt
{
	ManagerPrivate::ManagerPrivate(Responder::Generator responderGenerator, QObject* parent)
		:
			QObject(parent),
			m_socketNotifier(new QSocketNotifier(FCGI_LISTENSOCK_FILENO, QSocketNotifier::Read, this)),
			m_responderGenerator(responderGenerator)
	{
		connect(
			m_socketNotifier,
			SIGNAL(activated(int)),
			this,
			SLOT(listen())
		);

		// Spawn some threads
		for(int i = 0; i < qMax(QThread::idealThreadCount(), 1); ++i)
		{
			QThread* thread = new QThread(this);
			thread->start();
			m_threads.append(thread);
		}

		// Wait for the event loop to start up before running
		QTimer::singleShot(0, this, SLOT(listen()));
	}

	bool ManagerPrivate::hasLessLoadThan(QThread* t1, QThread* t2)
	{
		Q_ASSERT(t1->parent() == t2->parent());
		ManagerPrivate* p = qobject_cast<ManagerPrivate*>(t1->parent());
		Q_ASSERT(p);
		return p->m_threadLoads.value(t1) < p->m_threadLoads.value(t2);
	}

	void ManagerPrivate::listen()
	{
		// Initialise socket address structure
		sockaddr_un sa;
		socklen_t len = sizeof(sa);
		::memset(&sa, 0, len);

		// The recommended way of telling if we're running as fastcgi or not.
		int error = ::getpeername(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		if(error == -1 && errno != ENOTCONN)
		{
			qDebug() << tr("CGI not supported.");
			QCoreApplication::exit(-1);
			return;
		}

		// Listen on the socket
		lockSocket(FCGI_LISTENSOCK_FILENO);
		int newSocket = ::accept(FCGI_LISTENSOCK_FILENO, reinterpret_cast<sockaddr*>(&sa), &len);
		releaseSocket(FCGI_LISTENSOCK_FILENO);

		/* We're connected, setup a SocketManager.
		 * This will delete itself when appropriate (via deleteLater())
		 */
		// Pick a thread to put it in
		qSort(m_threads.begin(), m_threads.end(), hasLessLoadThan);
		QThread* thread = m_threads.first();
		m_threadLoads[thread].ref();
		SocketManager* socket = new SocketManager(m_responderGenerator, newSocket, NULL);
		connect(
			socket,
			SIGNAL(finished(QThread*)),
			this,
			SLOT(reduceLoadCount(QThread*))
		);
		socket->moveToThread(thread);
	}

	void ManagerPrivate::reduceLoadCount(QThread* thread)
	{
		m_threadLoads[thread].deref();
	}

	void ManagerPrivate::lockSocket(int socket)
	{
		::flock(socket, LOCK_EX);
	}

	void ManagerPrivate::releaseSocket(int socket)
	{
		::flock(socket, LOCK_UN);
	}
}
