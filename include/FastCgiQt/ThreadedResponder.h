#pragma once

#include "Responder.h"

namespace FastCgiQt
{
	/** Multithreaded dispatcher for FastCgiQt requests.
	 *
	 * @see BlockingResponder for advantages/disadvantages over using Manager::newRequest directly
	 *
	 * @section advantages_blocking Advantages over using BlockingResponder
	 *  - In some cases, may be faster
	 *
	 * @section disadvantages_blocking Disadvantages over using BlockingResponder
	 *  - In many cases, will be slow due to threading overhead
	 *  - Your code must be thread-safe
	 */
	class ThreadedResponder : public Responder
	{
		Q_OBJECT;
		public:
			ThreadedResponder(SpawnerBase* spawner, const char* responderSlot, QObject* parent = 0);
			~ThreadedResponder();
		public slots:
			void respond(FastCgiQt::Request*);
		private:
			class Private;
			Private* d;
	};
};
