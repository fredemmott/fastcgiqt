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
#include "Request.h"

namespace FastCgiQt
{
	class InputDevice;
	/** @internal Worker that handles one request.
	 * @fixme this maps the new plugin API to the current user and internal APIs.
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
