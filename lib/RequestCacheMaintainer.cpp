#include "RequestCacheMaintainer.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QMutexLocker>
#include <QWriteLocker>

namespace FastCgiQt
{
	QMutex RequestCacheMaintainer::m_instanceMutex;
	QHash<Cache*, QPointer<RequestCacheMaintainer> > RequestCacheMaintainer::m_instances;

	RequestCacheMaintainer* RequestCacheMaintainer::instance(Cache* cache)
	{
		QMutexLocker lock(&m_instanceMutex);
		if(!m_instances.contains(cache))
		{
			m_instances.insert(cache, new RequestCacheMaintainer(cache));
		}
		return m_instances.value(cache);
	}

	RequestCacheMaintainer::RequestCacheMaintainer(Cache* cache, QObject* parent)
		:
			QObject(parent),
			m_cache(cache),
			m_watcher(new QFileSystemWatcher(this))

	{
		connect(
			m_watcher,
			SIGNAL(fileChanged(QString)),
			this,
			SLOT(removeAssociatedEntries(QString))
		);
	}

	QList<QString> RequestCacheMaintainer::dependencies(const QString& urlFragment)
	{
		QMutexLocker lock(&m_watcherMutex);
		return m_filesToFragments.keys(urlFragment);
	}

	void RequestCacheMaintainer::addDependency(const QString& urlFragment, const QString& file)
	{
		QMutexLocker lock(&m_watcherMutex);
		if(!m_filesToFragments.contains(file))
		{
			m_watcher->addPath(file);
		}
		if(!m_filesToFragments.contains(file, urlFragment))
		{
			m_filesToFragments.insert(file, urlFragment);
		}
	}


	void RequestCacheMaintainer::removeAssociatedEntries(const QString& path)
	{
		QMutexLocker lock(&m_watcherMutex);
		m_watcher->removePath(path);

		QWriteLocker writeLock(m_cache->readWriteLock()); // so we don't have to re-acquire the lock for every removal
		Q_FOREACH(const QString& urlFragment, m_filesToFragments.values(path))
		{
			m_cache->remove(urlFragment);
		}
		m_filesToFragments.remove(path);
	}
}
