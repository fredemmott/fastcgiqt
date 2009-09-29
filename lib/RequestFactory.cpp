#include "RequestFactory.h"

#include "Request_Private.h"

namespace FastCgiQt
{
	Request* RequestFactory::createRequest(ClientIODevice* device, QObject* parent)
	{
		return new Request(new Request::Private(device), parent);
	}
};
