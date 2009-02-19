#ifndef _FASTCGIQT_CACHES_H
#define _FASTCGIQT_CACHES_H

#include "FileCache.h"

#include <QAtomicInt>
#include <QPointer>

namespace FastCgiQt
{
	/** Class providing access to global file/request caches.
	 *
	 * An instance of this class is used for reference counting and cleanup.
	 */
	class Caches
	{
		public:
			/// Increase the reference count.
			Caches();
			/// Decrease the reference count.
			~Caches();
			/// Get a reference to the file cache.
			static FileCache& fileCache();
			/// Get a reference to the reference cache.
			static FileDependentCache& requestCache();
		private:
			static QAtomicInt m_referenceCount;
			static QPointer<FileCache> m_fileCache;
			static QPointer<FileDependentCache> m_requestCache;
	};
}

#endif
