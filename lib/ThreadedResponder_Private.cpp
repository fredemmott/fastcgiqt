#include "ThreadedResponder_Private.h"

#include "Request.h"
#include "RequestRunner.h"
#include "ThreadPool.h"

#include <QTimer>

namespace FastCgiQt
{
	ThreadedResponder::Private::Private(SpawnerBase* spawner, const char* responderSlot, QObject* parent)
	: RequestQueue(spawner, responderSlot, parent)
	, m_threadPool(new ThreadPool(this))
	{
	}

	void ThreadedResponder::Private::start(RequestRunner* runner)
	{
		m_threadPool->start(runner, runner->request());
	}
}
