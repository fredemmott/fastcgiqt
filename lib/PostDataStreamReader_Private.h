#ifndef _FASTCGIQT_POSTDATASTREAMREADER_PRIVATE
#define _FASTCGIQT_POSTDATASTREAMREADER_PRIVATE

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
#endif // _FASTCGIQT_POSTDATASTREAMREADER_PRIVATE
