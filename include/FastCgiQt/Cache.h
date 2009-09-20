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
#pragma once

#include "CacheEntry.h"

class QReadWriteLock;

namespace FastCgiQt
{
	/** @brief A read-write-locked cache structure.
	 *
	 * Depending on the FastCgiQt configuration, this will use one of
	 * several backends; for example, QCache-based, or memcached-based.
	 *
	 * If QCache is used, the cache will be shared between all threads in
	 * the current process. If memcached is used, the cache will:
	 * - Perform better (honestly, even though it's out of process)
	 * - Be shared with any other instances of the same application using
	 *   the same memcached pool.
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
			class Private;
			Private* d;
	};
}
