#include "RamCache.h"

#include <QDebug>
#include <QReadLocker>
#include <QWriteLocker>

namespace FastCgiQt
{
	QCache<QString, CacheEntry> RamCache::m_cache(10*1024*1024);
	QReadWriteLock RamCache::m_lock(QReadWriteLock::Recursive);

	CacheBackend* RamCacheFactory::getCacheBackend() const
	{
		return new RamCache();
	}

	CacheEntry RamCache::value(const QString& key) const
	{
		QReadLocker lock(&m_lock);
		CacheEntry* pointer = m_cache.object(key);
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
		m_cache.insert(key, new CacheEntry(entry), entry.data().length());
	}

	void RamCache::remove(const QString& key)
	{
		QWriteLocker lock(&m_lock);
		m_cache.remove(key);
	}
}
