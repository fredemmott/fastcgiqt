#include "ThreadedResponder.h"
#include "ThreadedResponder_Private.h"

namespace FastCgiQt
{
	ThreadedResponder::ThreadedResponder(SpawnerBase* spawner, const char* responderSlot, QObject* parent)
	: AbstractResponder(parent)
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

	void ThreadedResponder::respond(SpawnerBase* spawner, const char* responderSlot, Request* request)
	{
		d->respond(spawner, responderSlot, request);
	}
};
