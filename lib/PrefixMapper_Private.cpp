#include "PrefixMapper_Private.h"
#include "Request.h"

#include <QVariant>

const char* PrefixTrackingProperty = "FASTCGIQT_PREFIXMAPPER_PREFIX";
const char* SuffixTrackingProperty = "FASTCGIQT_PREFIXMAPPER_SUFFIX";

namespace FastCgiQt
{
	void PrefixMapper::Private::setPrefix(Request* request, const QString& value)
	{
		request->setProperty(PrefixTrackingProperty, value);
	}

	void PrefixMapper::Private::setSuffix(Request* request, const QString& value)
	{
		request->setProperty(SuffixTrackingProperty, value);
	}

	QString PrefixMapper::Private::prefix(Request* request)
	{
		return request->property(PrefixTrackingProperty).toString();
	}

	QString PrefixMapper::Private::suffix(Request* request)
	{
		const QString suffix = request->property(SuffixTrackingProperty).toString();
		if(!suffix.isNull())
		{
			return request->value(ServerData, "PATH_INFO");
		}
	}
};
