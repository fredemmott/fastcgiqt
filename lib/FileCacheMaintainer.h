#ifndef _FASTCGI_QT_FILE_CACHE_MAINTAINER_H
#define _FASTCGI_QT_FILE_CACHE_MAINTAINER_H

#include "Cache.h"

#include <QHash>
#include <QMutex>
#include <QObject>
#include <QPointer>

class QFileSystemWatcher;

namespace FastCgiQt
{
	/** Class to remove entries from a Cache with filenames as keys when the
	 * files are modified.
	 *
	 * This class uses QFileSystemWatcher to watch all files in the cache,
	 * and remove them from the cache as soon as they are modified.
	 */
	class FileCacheMaintainer : public QObject, private Cache::Observer
	{
		Q_OBJECT
		public:
			/** Make sure that there's an instance of this class
			 * acting on the  specified cache.
			 */
			static FileCacheMaintainer* instance(Cache* cache);
		private slots:
			/** Remove an entry from the cache.
			 *
			 * This is called when a file is modified.
			 */
			void removeEntry(const QString& path);
		private:
			/** Called when an entry is added to a cache.
			 *
			 * This is an implementation of the Query::Observer interface.
			 */
			virtual void entryAdded(const QString& key, CacheEntry* entry);

			/** Construct a FileCacheMaintainer operating on the given cache.
			 *
			 * @see instance
			 */
			FileCacheMaintainer(Cache* cache, QObject* parent = NULL);

			Cache* m_cache;
			QFileSystemWatcher* m_watcher;
			QMutex m_watcherMutex;

			static QMutex m_instanceMutex;
			static QHash<Cache*, QPointer<FileCacheMaintainer> > m_instances;
	};
}

#endif
