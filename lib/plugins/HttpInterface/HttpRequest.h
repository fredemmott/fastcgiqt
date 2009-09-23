#pragma once

#include "CommunicationInterface_Worker.h"
#include "Responder.h"

#include <event.h>
#include <evhttp.h>

namespace FastCgiQt
{
	class HttpRequest : public CommunicationInterface::Worker
	{
		Q_OBJECT;
		public:
			HttpRequest(Responder::Generator generator, struct evhttp_request* request, QObject* parent);
			~HttpRequest();
	};
};
