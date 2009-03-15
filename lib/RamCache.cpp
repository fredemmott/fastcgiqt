#include "RamCache.h"

#include <QReadLocker>
#include <QWriteLocker>

namespace FastCgiQt
{
	QCache<QString, CacheEntry> RamCache::m_cache(10240);
	QReadWriteLock RamCache::m_lock;

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
