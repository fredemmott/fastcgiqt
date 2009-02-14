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
#ifndef _FASTCGI_QT_SERVICE_PRIVATE_H
#define _FASTCGI_QT_SERVICE_PRIVATE_H

#include <QCache>
#include <QList>
#include <QPair>
#include <QRegExp>

class QMetaMethod;

namespace FastCgiQt
{
	class Service::Private
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
			bool dispatchingRequest;
			static bool usingFileCache;
			static QCache<QString, QByteArray> fileCache;
	};
}

#endif
