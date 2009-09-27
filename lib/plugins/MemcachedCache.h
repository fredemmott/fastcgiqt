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
#ifndef _FASTCGI_QT_MEMCACHED_CACHE_H
#define _FASTCGI_QT_MEMCACHED_CACHE_H

#include "CacheBackend.h"

#include <QReadWriteLock>
#include <QList>

struct memcached_st;

namespace FastCgiQt
{
	/** @internal Cache backend implemented using memcached.
	 *
	 * Binary key format:
	 * 	UTF8 string
	 * Binary data format:
	 * 	little-endian 64-bit UTC timestamp, data
	 *
	 * @ingroup plugins
	 */
	class MemcachedCache : public CacheBackend
	{
		public:
			MemcachedCache(const QString& cacheName);
			~MemcachedCache();
			virtual void remove(const QString& key);
			virtual CacheEntry value(const QString& key) const;
			virtual void setValue(const QString& key, const CacheEntry& entry);
			virtual QReadWriteLock* readWriteLock() const;

			class Server
			{
				public:
					Server(const QString& host, unsigned int port);
					QString host() const;
					unsigned int port() const;
				private:
					QString m_host;
					unsigned int m_port;
			};
			static void setServers(const QList<Server>& servers);
		private:
			QByteArray fullKey(const QString& key) const;
			memcached_st* m_memcached;
			const QString m_keyPrefix;
			static QList<Server> m_servers;
			static QReadWriteLock m_lock;
	};

	class MemcachedCacheFactory : public QObject, public CacheBackend::Factory
	{
		Q_OBJECT
		Q_INTERFACES(FastCgiQt::CacheBackend::Factory)
		public:
			virtual bool loadSettings();
			virtual CacheBackend* createCacheBackend(const QString& cacheName) const;
	};
}

#endif
