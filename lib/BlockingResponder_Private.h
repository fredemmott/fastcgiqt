#pragma once

#include "BlockingResponder.h"
#include "RequestQueue.h"

#include <QQueue>

namespace FastCgiQt
{
	class BlockingResponder::Private : public RequestQueue
	{
		Q_OBJECT;
		public:
			Private(SpawnerBase* spawner, const char* responderSlot, QObject* parent);
		protected:
			void start(RequestRunner* runner);
	};
};
