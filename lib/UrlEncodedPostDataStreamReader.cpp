#include "UrlEncodedPostDataStreamReader.h"

namespace FastCgiQt
{
	UrlEncodedPostDataStreamReader(QIODevice* source)
	: PostDataStreamReaderBackend(source)
	{
	}

	UrlEncodedPostDataStreamReader::~UrlEncodedPostDataStreamReader()
	{
		delete m_content;
	}
};
