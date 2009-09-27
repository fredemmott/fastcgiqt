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
				WaitingForRequest,
				WaitingForRequestHeaders,
				WaitingForRequestBody
			};
			enum ResponseState
			{
				WaitingForResponseHeaders,
				WaitingForResponseBody
			};
			RequestState m_requestState;
			ResponseState m_responseState;

			HeaderMap m_requestHeaders;
			HeaderMap m_responseHeaders;

			QTcpSocket* m_socket;

			QStringList m_staticDirectories;
	};
};
