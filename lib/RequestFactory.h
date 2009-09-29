#pragma once

#include "Request.h"

namespace FastCgiQt
{
	class ClientIODevice;
	class RequestFactory : public Request
	{
		public:
			static Request* createRequest(ClientIODevice*, QObject* parent);
	};
};
