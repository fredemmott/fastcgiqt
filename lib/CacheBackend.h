#ifndef _FASTCGI_QT_CACHE_BACKEND_H
#define _FASTCGI_QT_CACHE_BACKEND_H

#include "CacheEntry.h"

#include <QtPlugin>

class QReadWriteLock;

namespace FastCgiQt
{
	/** Abstract interface for cache backends.
	 *
	 * Implementations are expected to use a QReadWriteLock as neccessary.
	 * If the implementation doesn't require it, a valid pointer should
	 * still be returned for readWriteLock in case it's required for
	 * higher-level classes (such as FileDependentCache).
	 *
	 * @see CacheBackend::Factory
	 *
	 * @ingroup plugins
	 */
	class CacheBackend
	{
		public:
			virtual ~CacheBackend();
			/// Remove an entry from the cache.
			virtual void remove(const QString& key) = 0;
			/// Fetch a value from the cache.
			virtual CacheEntry value(const QString& key) const = 0;
			/// Write a value to the cache.
			virtual void setValue(const QString& key, const CacheEntry& entry) = 0;

			/// Retrieve the read-write lock.
			virtual QReadWriteLock* readWriteLock() const = 0;

			/** Factory class constructing a CacheBackend.
			 *
			 * This is a qt plugin interface.
			 *
			 * @ingroup plugins
			 */
			class Factory
			{
				public:
					/** Load any settings relevant for this cache backend.
					 *
					 * If this doesn't create the current backend, it should return false.
					 *
					 * The settings file is an ini file called .applicationName in the working
					 * directory. The relevant key is cache/backend - if not set, RamCache is
					 * the default.
					 */
					virtual bool loadSettings() = 0;
					/// Create a CacheBackend*
					virtual CacheBackend* getCacheBackend(const QString& cacheName) const = 0;
			};
	};
}

Q_DECLARE_INTERFACE(FastCgiQt::CacheBackend::Factory, "uk.co.fredemmott.FastCgiQt.CacheBackend/1.0");

#endif
