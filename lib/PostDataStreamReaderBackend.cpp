#include "PostDataStreamReaderBackend.h"

namespace FastCgiQt
{
	PostDataStreamReaderBackend::PostDataStreamReaderBackend(QIODevice* source)
	: m_source(source)
	{
	}

	PostDataStreamReaderBackend::~PostDataStreamReaderBackend()
	{
	}
	
	bool PostDataStreamReaderBackend::atEnd() const
	{
		return source()->atEnd() && (tokenType() == PostDataStreamReader::Invalid || tokenType() == PostDataStreamReader::EndData);
	}

	QIODevice* PostDataStreamReaderBackend::source() const
	{
		return m_source;
	}
};
