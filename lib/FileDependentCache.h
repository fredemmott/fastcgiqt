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
			FileDependentCache(const QString& cacheName, QObject* parent = NULL);

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
