#ifndef _FASTCGI_QT_FILE_DEPENDENT_CACHE_H
#define _FASTCGI_QT_FILE_DEPENDENT_CACHE_H

#include "Cache.h"

#include <QHash>
#include <QMultiHash>
#include <QMutex>
#include <QObject>
#include <QPointer>

class QFileSystemWatcher;

namespace FastCgiQt
{
	/** @internal @brief Cache where entries depend on files.
	 *
	 * When a file is modified, all dependent entries are automatically removed.
	 */
	class FileDependentCache: public QObject, public Cache
	{
		Q_OBJECT
		public:
			/// Create a FileDependentCache containing up to @p maxSize bytes.
			FileDependentCache(QObject* parent = NULL);

			/// Add a dependency between a key and a file.
			void addDependency(const QString& key, const QString& file);
			/// Return a list of files dependent on by the specified key.
			QList<QString> dependencies(const QString& key);
		protected slots:
			/** Remove an entry from the cache.
			 *
			 * This is called when a file is modified.
			 */
			virtual void removeAssociatedEntries(const QString& path);
		protected:
			QFileSystemWatcher* m_watcher;
			QMutex m_watcherMutex;
		private:
			QMultiHash<QString, QString> m_filesToFragments;
	};
}

#endif
