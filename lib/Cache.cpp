#include "Cache.h"

#include "RamCache.h"

#include <QDebug>

namespace FastCgiQt
{
	Cache::Cache()
		:
			m_backend(new RamCache())
	{
	}

	Cache::~Cache()
	{
	}


	void Cache::remove(const QString& key)
	{
		m_backend->remove(key);
	}

	bool Cache::contains(const QString& key) const
	{
		if(m_cache.contains(key))
		{
			return true;
		}

		CacheEntry entry = m_backend->value(key);
		if(entry.isValid())
		{
			m_cache.insert(key, entry);
			return true;
		}
		else
		{
			return false;
		}
	}

	CacheEntry Cache::value(const QString& key) const
	{
		if(contains(key))
		{
			return m_cache.value(key);
		}
		else
		{
			return CacheEntry();
		}
	}

	void Cache::setValue(const QString& key, const CacheEntry& entry)
	{
		m_cache[key] = entry;
		m_backend->setValue(key, entry);
	}

	QReadWriteLock* Cache::readWriteLock() const
	{
		return m_backend->readWriteLock();
	}
}
