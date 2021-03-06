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
#include "MemcachedCache.h"
#include "Settings.h"

#include <libmemcached/memcached.h>

#include <QtEndian>
#include <QCoreApplication>
#include <QDebug>
#include <QReadLocker>
#include <QStringList>
#include <QWriteLocker>

namespace FastCgiQt
{
	QReadWriteLock MemcachedCache::m_lock(QReadWriteLock::Recursive);
	QList<MemcachedCache::Server> MemcachedCache::m_servers;

	MemcachedCache::MemcachedCache(const QString& cacheName)
		:
			m_memcached(memcached_create(NULL)),
			m_keyPrefix(QCoreApplication::organizationDomain() + '/' + QCoreApplication::applicationName() + '/' + cacheName + '/')
	{
		memcached_return rt;

		Q_FOREACH(const Server& server, m_servers)
		{
			rt = memcached_server_add(m_memcached, server.host().toLatin1().constData(), server.port());
			if(rt != MEMCACHED_SUCCESS)
			{
				qFatal("Failed to add memcached server %s:%d.", qPrintable(server.host()), server.port());
			}
		}
	}

	QByteArray MemcachedCache::fullKey(const QString& key) const
	{
		return QString(m_keyPrefix + key).toUtf8();
	}
	
	MemcachedCache::~MemcachedCache()
	{
		memcached_free(m_memcached);
	}

	void MemcachedCache::setServers(const QList<Server>& servers)
	{
		m_servers = servers;
	}

	CacheBackend* MemcachedCacheFactory::createCacheBackend(const QString& cacheName) const
	{
		return new MemcachedCache(cacheName);
	}

	MemcachedCache::Server::Server(const QString& host, unsigned int port)
		:
			m_host(host),
			m_port(port)
	{
	}

	QString MemcachedCache::Server::host() const
	{
		return m_host;
	}

	unsigned int MemcachedCache::Server::port() const
	{
		return m_port;
	}

	bool MemcachedCacheFactory::loadSettings()
	{
		Settings settings;
		if(settings.value("cache/backend") == "MemcachedCache")
		{
			QList<MemcachedCache::Server> servers;
			const int size = settings.beginReadArray("MemcachedServers");
			if(size == 0)
			{
				servers.append(MemcachedCache::Server("localhost", MEMCACHED_DEFAULT_PORT));
			}
			else
			{
				for(int i = 0; i < size; ++i)
				{
					settings.setArrayIndex(i);
					const QString host(settings.value("host").toString());
					if(!host.isNull())
					{
						const unsigned int port = settings.value("port", MEMCACHED_DEFAULT_PORT).value<unsigned int>();
						servers.append(MemcachedCache::Server(host, port));
					}
				}
			}
			settings.endArray();
			MemcachedCache::setServers(servers);
			return true;
		}
		return false;
	}

	CacheEntry MemcachedCache::value(const QString& key) const
	{
		QReadLocker lock(&m_lock);

		const QByteArray rawKey(fullKey(key));

		char* rawData;
		size_t rawDataLength;
		uint32_t flags;
		memcached_return rt;

		rawData = memcached_get(
			m_memcached,
			rawKey.constData(),
			rawKey.length(),
			&rawDataLength,
			&flags,
			&rt
		);

		if(rt != MEMCACHED_SUCCESS && rt != MEMCACHED_NOTFOUND)
		{
			qFatal("Memcached error: %s", memcached_strerror(m_memcached, rt));
		}

		Q_ASSERT(rawDataLength >= sizeof(quint64) || !rawData);
		if(rawDataLength < sizeof(quint64) || !rawData)
		{
			return CacheEntry();
		}
		
		const quint64 timeStamp = qFromLittleEndian(*reinterpret_cast<quint64*>(rawData));
		const QByteArray data(rawData + sizeof(quint64), rawDataLength - sizeof(quint64));
		return CacheEntry(QDateTime::fromTime_t(timeStamp), data);
	}

	QReadWriteLock* MemcachedCache::readWriteLock() const
	{
		return &m_lock;
	}

	void MemcachedCache::setValue(const QString& key, const CacheEntry& entry)
	{
		QWriteLocker lock(&m_lock);

		// Binary key
		const QByteArray rawKey(fullKey(key));

		// Binary data
		const quint64 dateTime(qToLittleEndian(static_cast<quint64>(entry.timeStamp().toTime_t())));
		QByteArray rawData(reinterpret_cast<const char*>(&dateTime), sizeof(quint64));
		rawData.append(entry.data());

		// Store in memcached
		memcached_return rt;
		rt = memcached_set(
			m_memcached,
			rawKey.constData(),
			rawKey.length(),
			rawData.constData(),
			rawData.length(),
			0, // expire
			0 // flags
		);
		if(rt != MEMCACHED_SUCCESS)
		{
			qFatal("Memcached error: %s", memcached_strerror(m_memcached, rt));
		}
	}

	void MemcachedCache::remove(const QString& key)
	{
		QWriteLocker lock(&m_lock);
		const QByteArray rawKey(fullKey(key));
		const memcached_return rt = memcached_delete(
			m_memcached,
			rawKey.constData(),
			rawKey.length(),
			0
		);
		if(rt != MEMCACHED_SUCCESS)
		{
			qFatal("Memcached error: %s", memcached_strerror(m_memcached, rt));
		}
	}
}

Q_EXPORT_PLUGIN2(FastCgiQt_MemcachedCache, FastCgiQt::MemcachedCacheFactory)
