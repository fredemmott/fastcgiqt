#pragma once

#include "CommunicationInterface_Worker.h"

class QTcpSocket;

namespace FastCgiQt
{
	class InputDevice;
	class ScgiRequest : public CommunicationInterface::Worker
	{
		Q_OBJECT;
		public:
			ScgiRequest(Responder::Generator generator, QTcpSocket* socket, QObject* parent);
			~ScgiRequest();
		private slots:
			void readData();
			void cleanup(Responder* responder);
		private:
			enum State
			{
				NewConnection,
				HaveHeaders
			};

			void readHeaders();
			void spawnResponder();

			Responder::Generator m_responderGenerator;
			InputDevice* m_inputDevice;
			QTcpSocket* m_socket;
			Request m_request;
			State m_state;
	};
};
