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
