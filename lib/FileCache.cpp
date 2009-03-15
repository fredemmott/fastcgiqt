#include "FileCache.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QMutexLocker>
#include <QStringList>

namespace FastCgiQt
{
	FileCache::FileCache(const QString& cacheName, QObject* parent)
		:
			FileDependentCache(cacheName, parent)

	{
	}

	void FileCache::setValue(const QString& key, const CacheEntry& entry)
	{
		FileDependentCache::setValue(key, entry);
		addDependency(key, key);
	}

	void FileCache::remove(const QString& path)
	{
		removeAssociatedEntries(path);
		return FileDependentCache::remove(path);
	}
}
