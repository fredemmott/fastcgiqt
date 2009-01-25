#ifndef _FASTCGI_QT_END_REQUEST_RECORD_H
#define _FASTCGI_QT_END_REQUEST_RECORD_H

#include <QByteArray>

namespace FastCgiQt
{
	class EndRequestRecord
	{
		public:
			static QByteArray create(quint16 requestId);
	};
}

#endif
