#pragma once

#include "Request.h"

namespace FastCgiQt
{
	class Request::Backend
	{
		public:
			Backend();
			/// Called by Request::Request(Backend*)
			void setRequest(Request* request);

			/// Add some newly-received server variables to this Request object.
			void addServerData(const QHash<QString, QString>& data);

			/// Add some newly-received post data.
			void appendContent(const QByteArray& data);

			// See documentation in Request.h
			
			QString contentType() const;
			quint64 contentLength() const;
			QByteArray content() const;

			QHash<QString, QString> postData() const;
			QHash<QString, QString> serverData() const;
			QHash<QString, QString> getData() const;
		private:
			QHash<QString, QString> m_serverData;
			QHash<QString, QString> m_getData;
			QHash<QString, QString> m_postData;

			QString m_contentType;
			quint16 m_contentLength;
			QByteArray m_content;

			Request* m_request;

	};
};
