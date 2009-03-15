#ifndef _FASTCGI_QT_CACHE_H
#define _FASTCGI_QT_CACHE_H

#include "CacheBackend.h"
#include "CacheEntry.h"
#include <QHash>

class QReadWriteLock;

namespace FastCgiQt
{
	/** @internal @brief A read-write-locked cache structure.
	 *
	 * This is a QCache<QString, CacheEntry> with a recursive QReadWriteLock.
	 *
	 * All members lock as neccessary, but you may need to lock at an outer
	 * scope depending on the usage.
	 *
	 * Keep in mind that even with a recursive lock, the locks must be of
	 * the same type; for example, all the writing calls will deadlock
	 * if called while a read lock is open.
	 */
	class Cache
	{
		public:
			Cache(const QString& cacheName);
			~Cache();

			void remove(const QString& key);
			CacheEntry value(const QString& key) const;
			void setValue(const QString& key, const CacheEntry& object);

			bool contains(const QString& key) const;
			static void setBackendFactory(CacheBackend::Factory* factory);
		protected:
			/// Return a pointer to the QReadWriteLock.
			QReadWriteLock* readWriteLock() const;
		private:
			static CacheBackend::Factory* m_backendFactory;
			CacheBackend* m_backend;
			mutable QHash<QString, CacheEntry> m_cache;
	};
}

#endif
