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
	/** Subclass of Cache specifically designed for keeping caches of local files.
	 *
	 * This class uses QFileSystemWatcher to watch all files in the cache,
	 * and remove them from the cache as soon as they are modified.
	 */
	class FileCache: public FileDependentCache
	{
		Q_OBJECT
		public:
			/// Create a FileCache, containing up to @p maxSize bytes.
			FileCache(int maxSize = 100, QObject* parent = NULL);
			/// Remove all entries from the cache, and stop watching them.
			void clear();
			/// Add an entry to the cache, and start watching it.
			bool insert(const QString& path, CacheEntry* object);
			/// Remove an entry and stop watching it.
		public slots:
			bool remove(const QString& path);
	};
}

#endif
