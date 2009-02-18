#include "FileCache.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QMutexLocker>
#include <QStringList>

namespace FastCgiQt
{
	FileCache::FileCache(int maxSize, QObject* parent)
		:
			FileDependentCache(maxSize, parent)

	{
	}

	void FileCache::clear()
	{
		QMutexLocker lock(&m_watcherMutex);
		Q_FOREACH(const QString& path, m_watcher->files())
		{
			m_watcher->removePath(path);
		}
		FileDependentCache::clear();
	}

	bool FileCache::insert(const QString& key, CacheEntry* entry)
	{
		bool success = FileDependentCache::insert(key, entry);
		addDependency(key, key);
		return success;
	}

	bool FileCache::remove(const QString& path)
	{
		removeAssociatedEntries(path);
		return FileDependentCache::remove(path);
	}
}
