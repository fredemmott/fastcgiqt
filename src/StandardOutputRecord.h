#ifndef _FASTCGI_QT_STANDARD_OUTPUT_RECORD_H
#define _FASTCGI_QT_STANDARD_OUTPUT_RECORD_H

#include <QByteArray>

namespace FastCgiQt
{
	class StandardOutputRecord
	{
		public:
			static QByteArray create(quint16 requestId, QByteArray data);
	};
};

#endif
