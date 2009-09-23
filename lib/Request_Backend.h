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
			bool haveAllContent() const;

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
