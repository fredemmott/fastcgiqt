#ifndef _FASTCGI_QT_REQUEST_CACHE_MAINTAINER_H
#define _FASTCGI_QT_REQUEST_CACHE_MAINTAINER_H

#include "Cache.h"

#include <QHash>
#include <QMultiHash>
#include <QMutex>
#include <QObject>
#include <QPointer>

class QFileSystemWatcher;

namespace FastCgiQt
{
	class RequestCacheMaintainer : public QObject
	{
		Q_OBJECT
		public:
			/** Make sure that there's an instance of this class
			 * acting on the specified cache.
			 */
			static RequestCacheMaintainer* instance(Cache* cache);
			void addDependency(const QString& urlFragment, const QString& file);
		private slots:
			/** Remove an entry from the cache.
			 *
			 * This is called when a file is modified.
			 */
			void removeAssociatedEntries(const QString& path);
		private:

			/** Construct a FileCacheMaintainer operating on the given cache.
			 *
			 * @see instance
			 */
			RequestCacheMaintainer(Cache* cache, QObject* parent = NULL);

			Cache* m_cache;
			QFileSystemWatcher* m_watcher;
			QMutex m_watcherMutex;
			QMultiHash<QString, QString> m_filesToFragments;

			static QMutex m_instanceMutex;
			static QHash<Cache*, QPointer<RequestCacheMaintainer> > m_instances;
	};
}

#endif
