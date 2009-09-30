#pragma once

#include "ThreadedResponder.h"

#include <QRunnable>

namespace FastCgiQt
{
	/**@internal Class to run a Request.
	 *
	 * @see ThreadedResponder
	 */
	class RequestRunner : public QRunnable
	{
		public:
			RequestRunner(SpawnerBase* spawner, const char* responderSlot, Request* request);
			~RequestRunner();
			void run();
			Request* request() const;
		private:
			SpawnerBase* m_spawner;
			const char* m_slot;
			Request* m_request;
	};
};
