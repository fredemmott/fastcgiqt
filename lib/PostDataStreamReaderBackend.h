#pragma once

#include "PostDataStreamReader.h"

namespace FastCgiQt
{
	class PostDataStreamReaderBackend
	{
		public:
			PostDataStreamReaderBackend(QIODevice* source);
			virtual ~PostDataStreamReaderBackend();
			virtual bool atEnd() const;
			QIODevice* source() const;

			virtual PostDataStreamReader::TokenType tokenType() const = 0;
			virtual PostDataStreamReader::TokenType readNext() const = 0;
			virtual QString variableName() const = 0;
			virtual QIODevice* content() const = 0;
			virtual QString filename() const = 0;
			virtual QString mimeType() const = 0;
		private:
			QIODevice* m_source;
	};
};
