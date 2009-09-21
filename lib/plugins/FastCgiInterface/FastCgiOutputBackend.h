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

namespace FastCgiQt
{
	class FastCgiOutputBackend : public OutputDevice::Backend
	{
		Q_OBJECT;
		public:
			/// Will be reparented to the OutputDevice
			FastCgiOutputBackend(quint16 requestId, QIODevice* socket);
			/// Wait on the socket
			bool waitForBytesWritten(int msecs);
		protected:
			/// Write a FastCGI STDOUT record
			qint64 writeData(const char* data, qint64 maxSize);
		private:
			quint16 m_requestId;
			QIODevice* m_socket;
	};
}
