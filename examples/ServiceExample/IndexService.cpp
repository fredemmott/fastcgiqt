#include "IndexService.h"

IndexService::IndexService(const FastCgiQt::Request& request, QObject* parent)
	: Service(request, parent)
{
}

void IndexService::index()
{
	out << "Index goes here";
}

FastCgiQt::Service::UrlMap IndexService::urlMap() const
{
	FastCgiQt::Service::UrlMap map;
	map.append("", SLOT(index()));
	return map;
}
