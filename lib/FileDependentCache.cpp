#include "FileDependentCache.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QMutexLocker>
#include <QStringList>
#include <QWriteLocker>

namespace FastCgiQt
{
	FileDependentCache::FileDependentCache(const QString& cacheName, QObject* parent)
		:
			QObject(parent),
			Cache(cacheName),
			m_watcher(new QFileSystemWatcher(this)),
			m_watcherMutex(QMutex::Recursive)

	{
		connect(
			m_watcher,
			SIGNAL(fileChanged(QString)),
			this,
			SLOT(removeAssociatedEntries(QString))
		);
	}

	QList<QString> FileDependentCache::dependencies(const QString& urlFragment)
	{
		QMutexLocker lock(&m_watcherMutex);
		return m_filesToFragments.keys(urlFragment);
	}

	void FileDependentCache::addDependency(const QString& urlFragment, const QString& file)
	{
		QMutexLocker lock(&m_watcherMutex);
		if(!m_watcher->files().contains(file))
		{
			m_watcher->addPath(file);
		}
		if(!m_filesToFragments.contains(file, urlFragment))
		{
			m_filesToFragments.insert(file, urlFragment);
		}
	}


	void FileDependentCache::removeAssociatedEntries(const QString& path)
	{
		QMutexLocker lock(&m_watcherMutex);
		m_watcher->removePath(path);

		QWriteLocker writeLock(readWriteLock()); // so we don't have to re-acquire the lock for every removal
		Q_FOREACH(const QString& urlFragment, m_filesToFragments.values(path))
		{
			remove(urlFragment);
		}
		m_filesToFragments.remove(path);
	}
}
