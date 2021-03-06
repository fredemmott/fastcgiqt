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
#ifndef _FASTCGIQT_FASTCGISTREAM
#define _FASTCGIQT_FASTCGISTREAM

#include "ClientIODevice.h"

class QTcpSocket;

namespace FastCgiQt
{
	class FastCgiStream : public ClientIODevice
	{
		Q_OBJECT
		public:
			FastCgiStream(const HeaderMap& headers, quint16 requestId, QTcpSocket* output, QObject* parent);
			~FastCgiStream();

			bool waitForBytesWritten(int msec);
			qint64 bytesAvailable() const;

			HeaderMap requestHeaders() const;

			/** Data needs to be appended by the socket manager, not directly read from the socket,
			 * as FastCGI supports multiplexing.
			 */
			void appendData(const QByteArray& data);
		protected:
			/// Read data from buffer
			qint64 readData(char* data, qint64 maxSize);
			/// Write a FastCGI STDOUT record
			qint64 writeData(const char* data, qint64 maxSize);
		private:
			HeaderMap m_requestHeaders;
			QByteArray m_requestBuffer;
			qint64 m_requestBufferReadPosition;
			quint16 m_requestId;
			QTcpSocket* m_socket;
	};
}
#endif // _FASTCGIQT_FASTCGISTREAM
