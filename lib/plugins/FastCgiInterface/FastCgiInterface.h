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
#pragma once

#include "CommunicationInterface.h"

#include "Responder.h"

#include <QAtomicInt>
#include <QMap>
#include <QObject>
#include <QStringList>

class QFileSystemWatcher;
class QSocketNotifier;
class QThread;

namespace FastCgiQt
{
	class FastCgiSocketManager;
	/** @internal
	 * @brief Class managing new FastCGI connections.
	 *
	 * This class listens to the main FastCGI socket, spawns new
	 * connections, and passes them off to a FastCgiSocketManager.
	 *
	 * It will spread the load over QThread::idealThreadCount() threads.
	 */
	class FastCgiInterface : public CommunicationInterface
	{
		Q_OBJECT
		public:
			/// Create a FastCgiInterface object.
			FastCgiInterface(Responder::Generator responderGenerator, QObject* parent = NULL);
			~FastCgiInterface();
			bool start();
		private slots:
			/// Request that the application shuts down.
			void shutdown();
			/// Listen for a new FastCGI connection.
			void listen();
			/// Decrease the load counter for the specified thread.
			void reduceLoadCount(QThread* thread);
		private:
			QList<int> threadLoads() const;
			/// Lock the socket with the specified socket id.
			void lockSocket(int socket);
			/// Unlock the socket with the specified socket id.
			void releaseSocket(int socket);
			/// If we're shutting down, and the loads are zero, exit.
			void exitIfFinished();
			/** Comparison for thread loads.
			 *
			 * @returns true if thread @p t1 is currently handling
			 * 	less requests than @p t2.
			 * @returns false otherwise.
			 */
			static bool hasLessLoadThan(QThread* t1, QThread* t2);

			/// Socket handle
			int m_socket;

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

			/// The thread pool.
			QList<QThread*> m_threads;

			/** The number of requests each thread is currently
			 * handling.
			 */
			QMap<const QObject*, QAtomicInt> m_threadLoads;
	};
};
