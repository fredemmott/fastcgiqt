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

#include "OutputDevice_Backend.h"

#include <event.h>
#include <evhttp.h>

namespace FastCgiQt
{
	class HttpOutputBackend : public OutputDevice::Backend
	{
		Q_OBJECT;
		public:
			/// Will be reparented to the OutputDevice
			HttpOutputBackend(struct evhttp_request* request);
			~HttpOutputBackend();
		protected:
			qint64 writeData(const char* data, qint64 maxSize);
		private:
			enum State
			{
				NewReply,
				SentHeaders
			};

			struct evhttp_request* m_request;
			State m_state;
	};
}
