#pragma once

#include "PostDataStreamReader.h"

#include <QIODevice>
#include <QPointer>

namespace FastCgiQt
{
	class PostDataStreamReaderBackend
	{
		public:
			PostDataStreamReaderBackend(QIODevice* source);
			virtual ~PostDataStreamReaderBackend();
			virtual bool atEnd() const;
			QIODevice* source() const;

			PostDataStreamReader::TokenType tokenType() const;
			QString variableName() const;
			QString variableValue() const;
			QIODevice* content() const;
			QString filename() const;
			QString mimeType() const;

			virtual PostDataStreamReader::TokenType readNext() = 0;
		protected:
			PostDataStreamReader::TokenType m_tokenType;
			QString m_variableName;
			QString m_variableValue;
			QPointer<QIODevice> m_content;
			QString m_filename;
			QString m_mimetype;
		private:
			QIODevice* m_source;
	};
};
