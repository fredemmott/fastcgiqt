/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
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
