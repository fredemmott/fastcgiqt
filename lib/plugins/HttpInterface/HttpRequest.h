#pragma once

#include "CommunicationInterface_Worker.h"
#include "Request.h"
#include "Responder.h"

struct evhttp_request;

namespace FastCgiQt
{
	class InputDevice;
	class HttpRequest : public CommunicationInterface::Worker
	{
		Q_OBJECT;
		public:
			HttpRequest(Responder::Generator generator, struct evhttp_request* request, QObject* parent);
			~HttpRequest();
		private slots:
			void start();
			void cleanup(Responder* responder);
		private:
			Responder::Generator m_generator;
			struct evhttp_request* m_httpRequest;
			InputDevice* m_inputDevice;
			Request m_request;
	};
};
