#include "RequestDataProvider.h"

#include "Request.h"

namespace FastCgiQt
{
	void RequestDataProvider::addServerData(
		Request* request,
		const QHash<QString, QString>& data
	)
	{
		request->addServerData(data);
	}

	void RequestDataProvider::appendContent(
		Request* request,
		const QByteArray& data)
	{
		request->appendContent(data);
	}
}
