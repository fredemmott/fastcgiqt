#include "FileCache.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QMutexLocker>
#include <QStringList>

namespace FastCgiQt
{
	FileCache::FileCache(int maxSize, QObject* parent)
		:
			QObject(parent),
			Cache(maxSize),
			m_watcher(new QFileSystemWatcher(this))

	{
		connect(
			m_watcher,
			SIGNAL(fileChanged(QString)),
			this,
			SLOT(remove(QString))
		);
	}

	void FileCache::clear()
	{
		QMutexLocker lock(&m_watcherMutex);
		Q_FOREACH(const QString& path, m_watcher->files())
		{
			m_watcher->removePath(path);
		}
		Cache::clear();
	}

	bool FileCache::insert(const QString& key, CacheEntry* entry)
	{
		QMutexLocker lock(&m_watcherMutex);
		Q_UNUSED(entry);
		if(!m_watcher->files().contains(key))
		{
			m_watcher->addPath(key);
		}
		return Cache::insert(key, entry);
	}

	bool FileCache::remove(const QString& path)
	{
		QMutexLocker lock(&m_watcherMutex);
		m_watcher->removePath(path);
		return Cache::remove(path);
	}
}
