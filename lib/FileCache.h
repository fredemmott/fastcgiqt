#ifndef _FASTCGI_QT_FILE_CACHE_H
#define _FASTCGI_QT_FILE_CACHE_H

#include "Cache.h"

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
	class FileCache: public QObject, public Cache
	{
		Q_OBJECT
		public:
			/// Create a FileCache, containing up to @p maxSize bytes.
			FileCache(int maxSize = 100, QObject* parent = NULL);
			/// Remove all entries from the cache, and stop watching them.
			void clear();
			/// Add an entry to the cache, and start watching it.
			bool insert(const QString& path, CacheEntry* object);
		public slots:
			/// Remove an entry from the cache, and start watching it.
			bool remove(const QString& path);
		private:
			QFileSystemWatcher* m_watcher;
			QMutex m_watcherMutex;
	};
}

#endif
