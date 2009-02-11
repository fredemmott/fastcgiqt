#include "ServiceExample.h"

#include "ArticleService.h"
#include "IndexService.h"

#include <FastCgiQt/Service.h>

void ServiceExample::loadServices()
{
	addService("", IndexService::create);
	addService("articles", ArticleService::create);
}
