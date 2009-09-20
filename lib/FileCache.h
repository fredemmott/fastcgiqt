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
#ifndef _FASTCGI_QT_FILE_CACHE_H
#define _FASTCGI_QT_FILE_CACHE_H

#include "FileDependentCache.h"

#include <QHash>
#include <QMutex>
#include <QObject>
#include <QPointer>

class QFileSystemWatcher;

namespace FastCgiQt
{
	/** @internal @brief Subclass of Cache specifically designed for keeping caches of local files.
	 *
	 * This class uses QFileSystemWatcher to watch all files in the cache,
	 * and remove them from the cache as soon as they are modified.
	 */
	class FileCache: public FileDependentCache
	{
		Q_OBJECT
		public:
			/// Create a FileCache.
			FileCache(const QString& cacheName, QObject* parent = NULL);
			/// Add an entry to the cache, and start watching it.
			void setValue(const QString& path, const CacheEntry& object);
		public slots:
			/// Remove an entry and stop watching it.
			void remove(const QString& path);
	};
}

#endif
