#include "ThreadedResponder.h"
#include "ThreadedResponder_Private.h"

namespace FastCgiQt
{
	ThreadedResponder::ThreadedResponder(SpawnerBase* spawner, const char* responderSlot, QObject* parent)
	: Responder(parent)
	, d(new Private(spawner, responderSlot, this))
	{
	}

	ThreadedResponder::~ThreadedResponder()
	{
	}

	void ThreadedResponder::respond(FastCgiQt::Request* request)
	{
		d->respond(request);
	}
};
