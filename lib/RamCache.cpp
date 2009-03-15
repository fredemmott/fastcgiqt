#include "RamCache.h"

#include <QDebug>
#include <QReadLocker>
#include <QWriteLocker>

namespace FastCgiQt
{
	QCache<QString, CacheEntry> RamCache::m_cache(10*1024*1024);
	QReadWriteLock RamCache::m_lock(QReadWriteLock::Recursive);

	RamCache::RamCache(const QString& cacheName)
		:
			m_keyPrefix(cacheName + "::")
	{
	}

	CacheBackend* RamCacheFactory::getCacheBackend(const QString& cacheName) const
	{
		return new RamCache(cacheName);
	}

	CacheEntry RamCache::value(const QString& key) const
	{
		QReadLocker lock(&m_lock);
		CacheEntry* pointer = m_cache.object(m_keyPrefix + key);
		if(pointer)
		{
			return CacheEntry(*pointer);
		}
		else
		{
			return CacheEntry();
		}
	}

	QReadWriteLock* RamCache::readWriteLock() const
	{
		return &m_lock;
	}

	void RamCache::setValue(const QString& key, const CacheEntry& entry)
	{
		QWriteLocker lock(&m_lock);
		m_cache.insert(m_keyPrefix + key, new CacheEntry(entry), entry.data().length());
	}

	void RamCache::remove(const QString& key)
	{
		QWriteLocker lock(&m_lock);
		m_cache.remove(m_keyPrefix + key);
	}
}
