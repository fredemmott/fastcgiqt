#ifndef _FASTCGIQT_REQUEST_DATA_PROVIDER_H
#define _FASTCGIQT_REQUEST_DATA_PROVIDER_H

#include <QByteArray>
#include <QHash>
#include <QString>

namespace FastCgiQt
{
	class Request;
	class RequestDataProvider
	{
		public:
			static void addServerData(
				Request* request,
				const QHash<QString, QString>& data
			);
			static void appendContent(
				Request* request,
				const QByteArray& data
			);
	};
}

#endif
