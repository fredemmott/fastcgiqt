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

#include "ClientIODevice.h"

#include <QStringList>

class QTcpSocket;

namespace FastCgiQt
{
	class HttpRequest : public ClientIODevice
	{
		Q_OBJECT;
		public:
			HttpRequest(const HeaderMap& standardRequestHeaders, const HeaderMap& standardResponseHeaders, const QStringList& staticDirectories, QTcpSocket* socket, QObject* parent);
			~HttpRequest();
			HeaderMap requestHeaders() const;
		signals:
			void gotHeaders(HttpRequest*);
		protected:
			qint64 readData(char* data, qint64 maxSize);
			qint64 writeData(const char* data, qint64 maxSize);
		private slots:
			void readSocketData();
		private:
			/** We've got all the headers, dispatch the request.
			 *
			 * This either passes off the request to FastCgiQt, or, if it's in
			 * m_staticDirectories, serves the file raw.
			 */
			void dispatchRequest();
			enum RequestState
			{
				WaitingForRequest, ///< We've not actually received the "$VERB $URL HTTP/$VERSION" line yet.
				WaitingForRequestHeaders, ///< We might have recieved some headers, but not all of them (i.e. no \r\n\r\n yet).
				WaitingForRequestBody ///< Not actually expecting anything new; if we get it, it's POST data.
			};
			enum ResponseState
			{
				WaitingForResponseHeaders, ///< We've not received a \r\n from FastCgiQt yet
				WaitingForResponseBody ///< We're dealing with content now
			};
			RequestState m_requestState;
			ResponseState m_responseState;

			HeaderMap m_requestHeaders;
			HeaderMap m_responseHeaders;

			QTcpSocket* m_socket;

			QStringList m_staticDirectories;

			QByteArray m_headerBuffer;
			qint64 m_headerBufferPosition;
	};
};
