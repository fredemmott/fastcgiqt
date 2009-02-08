#ifndef _FASTCGI_QT_SERVICE_PRIVATE_H
#define _FASTCGI_QT_SERVICE_PRIVATE_H

#include <QList>
#include <QPair>
#include <QRegExp>

class QMetaMethod;

namespace FastCgiQt
{
	class ServicePrivate
	{
		public:
			typedef QPair<QRegExp, QMetaMethod> UrlMapEntry;
			QList<UrlMapEntry> forwardMap;
			void fillMap(Service* service);
			void invokeMethod(
				QObject* object,
				const QMetaMethod& method,
				const QStringList& parameters
			);

	};
}

#endif
