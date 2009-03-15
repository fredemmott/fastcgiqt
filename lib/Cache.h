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
	 * This is can use multiple backends, but currently uses only a
	 * QCache-based backend.
	 *
	 * All backends should lock as neccessary, but you may want to lock at
	 * an outer scope for performance.
	 *
	 * Keep in mind that even with a recursive lock, the locks must be of
	 * the same type; for example, all the writing calls will deadlock
	 * if called while a read lock is open.
	 *
	 * @ingroup plugins
	 */
	class Cache
	{
		public:
			/// Construct a Cache with the given name.
			Cache(const QString& cacheName);
			/// Destroy a cache.
			~Cache();

			/// Remove a key from the cache.
			void remove(const QString& key);

			/** Retrieve a value from the cache.
			 *
			 * If the entry isnt' in the cache, an invalid
			 * CacheEntry will be returned.
			 */
			CacheEntry value(const QString& key) const;

			/** Set a value in the cache.
			 *
			 * If a value already exists with the given key, it is
			 * overwritten.
			 */
			void setValue(const QString& key, const CacheEntry& object);
		protected:
			/// Return a pointer to the QReadWriteLock.
			QReadWriteLock* readWriteLock() const;
		private:
			/// Load the backend for the cache.
			void loadBackendFactory();
			static CacheBackend::Factory* m_backendFactory;
			CacheBackend* m_backend;
	};
}

#endif
