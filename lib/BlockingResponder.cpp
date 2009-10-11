#include "BlockingResponder.h"
#include "BlockingResponder_Private.h"

namespace FastCgiQt
{
	BlockingResponder::BlockingResponder(SpawnerBase* spawner, const char* responderSlot, QObject* parent)
	: AbstractResponder(parent)
	, d(new Private(spawner, responderSlot, this))
	{
	}

	BlockingResponder::~BlockingResponder()
	{
	}

	void BlockingResponder::respond(Request* request)
	{
		d->respond(request);
	}

	void BlockingResponder::respond(SpawnerBase* spawner, const char* responderSlot, Request* request)
	{
		d->respond(spawner, responderSlot, request);
	}
};
