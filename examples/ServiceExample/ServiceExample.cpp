#include "ServiceExample.h"

#include "ArticleService.h"
#include "IndexService.h"

#include <FastCgiQt/Service.h>

void ServiceExample::loadServices()
{
	addService("", new IndexService());
	addService("articles", new ArticleService());
}
