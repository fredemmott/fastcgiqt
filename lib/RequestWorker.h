#pragma once

#include "CommunicationInterface_Worker.h"
#include "Request.h"

namespace FastCgiQt
{
	class InputDevice;
	/** Worker that handles one request.
	 * FIXME this maps the new plugin API to the current user and internal APIs.
	 * These (at least the internal ones) should really be changed to deal with ClientIODevice*
	 * more directly.
	 */
	class RequestWorker : public CommunicationInterface::Worker
	{
		Q_OBJECT;
		public:
			RequestWorker(ClientIODevice* device, Responder::Generator generator, QObject* parent = 0);
			~RequestWorker();
		private slots:
			void start();
			void cleanupResponder(Responder*);
			void copyPostData();
		private:
			ClientIODevice* m_device;
			Responder::Generator m_generator;
			InputDevice* m_inputDevice;
			Request m_request;
	};
};
