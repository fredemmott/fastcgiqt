#pragma once

#include "PostDataStreamReader.h"
#include "PostDataStreamReaderBackend.h"

namespace FastCgiQt
{
	class PostDataStreamReader::Private
	{
		public:
			PostDataStreamReaderBackend* m_backend;
	};
};
