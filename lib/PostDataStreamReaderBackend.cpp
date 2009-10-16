#include "PostDataStreamReaderBackend.h"

namespace FastCgiQt
{
	PostDataStreamReaderBackend::PostDataStreamReaderBackend(QIODevice* source)
	: m_tokenType(PostDataStreamReader::NoToken)
	, m_source(source)
	{
	}

	PostDataStreamReaderBackend::~PostDataStreamReaderBackend()
	{
	}
	
	bool PostDataStreamReaderBackend::atEnd() const
	{
		return source()->atEnd() && (tokenType() == PostDataStreamReader::Invalid || tokenType() == PostDataStreamReader::EndData);
	}

	PostDataStreamReader::TokenType PostDataStreamReaderBackend::tokenType() const
	{
		return m_tokenType;
	}

	QString PostDataStreamReaderBackend::variableName() const
	{
		return m_variableName;
	}

	QIODevice* PostDataStreamReaderBackend::source() const
	{
		return m_source;
	}
};
