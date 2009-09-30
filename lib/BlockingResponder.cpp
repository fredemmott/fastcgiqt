#include "BlockingResponder.h"
#include "BlockingResponder_Private.h"

namespace FastCgiQt
{
	BlockingResponder::BlockingResponder(SpawnerBase* spawner, const char* responderSlot, QObject* parent)
	: Responder(parent)
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
};
