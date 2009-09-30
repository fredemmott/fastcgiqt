#pragma once

#include "RequestQueue.h"
#include "ThreadedResponder.h"

namespace FastCgiQt
{
	class ThreadPool;
	class ThreadedResponder::Private : public RequestQueue
	{
		Q_OBJECT;
		public:
			Private(SpawnerBase* spawner, const char* responderSlot, QObject* parent);
		protected:
			void start(RequestRunner* runner);
		private:
			ThreadPool* m_threadPool;
	};
};
