#pragma once

#include "PostDataStreamReaderBackend.h"

#include <QIODevice>
#include <QPointer>

namespace FastCgiQt
{
	class UrlEncodedPostDataStreamReader : public PostDataStreamReaderBackend
	{
		public:
			UrlEncodedPostDataStreamReader(QIODevice* source);
			virtual ~UrlEncodedPostDataStreamReader();
			virtual PostDataStreamReader::TokenType tokenType() const;
			virtual PostDataStreamReader::TokenType readNext() const;
			virtual QString variableName() const;
			virtual QIODevice* content() const;
			virtual QString filename() const;
			virtual QString mimeType() const;
		private:
			PostDataStreamReader::TokenType m_tokenType;
			QString m_variableName;
			QPointer<QIODevice> m_content;
			QString m_filename;
			QString m_mimetype;
	};
};
