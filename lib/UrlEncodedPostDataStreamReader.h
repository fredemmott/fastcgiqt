#pragma once

#include "PostDataStreamReaderBackend.h"

namespace FastCgiQt
{
	class UrlEncodedPostDataStreamReader : public PostDataStreamReaderBackend
	{
		public:
			UrlEncodedPostDataStreamReader(QIODevice* source);
			virtual ~UrlEncodedPostDataStreamReader();
			virtual PostDataStreamReader::TokenType readNext();
		private:
			int readUntil(char character, char alternative = 0);
			QByteArray m_buffer;
	};
};
