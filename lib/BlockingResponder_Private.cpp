#include "BlockingResponder_Private.h"

#include "Request.h"
#include "RequestRunner.h"

#include <QTimer>

namespace FastCgiQt
{
	BlockingResponder::Private::Private(SpawnerBase* spawner, const char* responderSlot, QObject* parent)
	: RequestQueue(spawner, responderSlot, parent)
	{
	}

	void BlockingResponder::Private::start(RequestRunner* runner)
	{
		runner->run();
		if(runner->autoDelete())
		{
			delete runner;
		}
	}
}
