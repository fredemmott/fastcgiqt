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
#ifndef _FASTCGI_QT_CACHE_BACKEND_H
#define _FASTCGI_QT_CACHE_BACKEND_H

#include "CacheEntry.h"

#include <QtPlugin>

class QReadWriteLock;

namespace FastCgiQt
{
	/** @internal Abstract interface for cache backends.
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

			/** @internal Factory class constructing a CacheBackend.
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
					virtual CacheBackend* createCacheBackend(const QString& cacheName) const = 0;
			};
	};
}

Q_DECLARE_INTERFACE(FastCgiQt::CacheBackend::Factory, "uk.co.fredemmott.FastCgiQt.CacheBackend/1.0");

#endif
