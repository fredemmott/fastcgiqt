#include "UrlEncodedPostDataStreamReader.h"

namespace FastCgiQt
{
	UrlEncodedPostDataStreamReader::UrlEncodedPostDataStreamReader(QIODevice* source)
	: PostDataStreamReaderBackend(source)
	{
	}

	UrlEncodedPostDataStreamReader::~UrlEncodedPostDataStreamReader()
	{
		delete m_content;
	}

	PostDataStreamReader::TokenType UrlEncodedPostDataStreamReader::readNext()
	{
		///@todo unimplemented
		return m_tokenType;
	}
};
