#include "HttpRequest.h"

namespace FastCgiQt
{
	HttpRequest::HttpRequest(Responder::Generator generator, struct evhttp_request* request, QObject* parent)
	: CommunicationInterface::Worker(parent)
	{
	}

	HttpRequest::~HttpRequest()
	{
	}
};
