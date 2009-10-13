/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include "PrefixMapper_Private.h"
#include "Request.h"

#include <QVariant>

const char* PrefixTrackingProperty = "FASTCGIQT_PREFIXMAPPER_PREFIX";
const char* SuffixTrackingProperty = "FASTCGIQT_PREFIXMAPPER_SUFFIX";

namespace FastCgiQt
{
	void PrefixMapper::Private::setPrefix(Request* request, const QByteArray& value)
	{
		request->setProperty(PrefixTrackingProperty, value);
	}

	void PrefixMapper::Private::setSuffix(Request* request, const QByteArray& value)
	{
		request->setProperty(SuffixTrackingProperty, value);
	}

	QByteArray PrefixMapper::Private::prefix(Request* request)
	{
		return request->property(PrefixTrackingProperty).toByteArray();
	}

	QByteArray PrefixMapper::Private::suffix(Request* request)
	{
		const QByteArray suffix = request->property(SuffixTrackingProperty).toByteArray();
		if(suffix.isNull())
		{
			return request->rawValue(ServerData, "PATH_INFO");
		}
		return suffix;
	}
};
