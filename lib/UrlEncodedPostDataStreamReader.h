#ifndef _FASTCGIQT_URLENCODEDPOSTDATASTREAMREADER
#define _FASTCGIQT_URLENCODEDPOSTDATASTREAMREADER

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
			qint64 m_bytesRead;
	};
};
#endif // _FASTCGIQT_URLENCODEDPOSTDATASTREAMREADER
