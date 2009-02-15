#include "FileCacheMaintainer.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QMutexLocker>

namespace FastCgiQt
{
	QMutex FileCacheMaintainer::m_instanceMutex;
	QHash<Cache*, QPointer<FileCacheMaintainer> > FileCacheMaintainer::m_instances;

	FileCacheMaintainer* FileCacheMaintainer::instance(Cache* cache)
	{
		QMutexLocker lock(&m_instanceMutex);
		if(!m_instances.contains(cache))
		{
			m_instances.insert(cache, new FileCacheMaintainer(cache));
		}
		return m_instances.value(cache);
	}

	FileCacheMaintainer::FileCacheMaintainer(Cache* cache, QObject* parent)
		:
			QObject(parent),
			m_cache(cache),
			m_watcher(new QFileSystemWatcher(this))

	{
		connect(
			m_watcher,
			SIGNAL(fileChanged(QString)),
			this,
			SLOT(removeEntry(QString))
		);
		cache->addObserver(this);
	}

	void FileCacheMaintainer::entryAdded(const QString& key, CacheEntry* entry)
	{
		QMutexLocker lock(&m_watcherMutex);
		Q_UNUSED(entry);
		m_watcher->addPath(key);
	}

	void FileCacheMaintainer::removeEntry(const QString& path)
	{
		QMutexLocker lock(&m_watcherMutex);
		m_cache->remove(path);
		m_watcher->removePath(path);
	}
}
