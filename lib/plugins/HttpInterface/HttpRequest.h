#pragma once

#include "ClientIODevice.h"

class QTcpSocket;

namespace FastCgiQt
{
	class HttpRequest : public ClientIODevice
	{
		Q_OBJECT;
		public:
			HttpRequest(const HeaderMap& standardRequestHeaders, const HeaderMap& standardResponseHeaders, QTcpSocket* socket, QObject* parent);
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
	};
};
