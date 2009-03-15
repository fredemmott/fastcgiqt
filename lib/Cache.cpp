#include "Cache.h"

#include <QDebug>
#include <QPluginLoader>

Q_IMPORT_PLUGIN(FastCgiQt_CacheBackend_RamCache)

namespace FastCgiQt
{
	CacheBackend::Factory* Cache::m_backendFactory(NULL);

	Cache::Cache(const QString& cacheName)
	{
		if(!m_backendFactory)
		{
			loadBackendFactory();
		}
		m_backend = m_backendFactory->getCacheBackend(cacheName);
	}

	void Cache::loadBackendFactory()
	{
		QList<CacheBackend::Factory*> factories;
		Q_FOREACH(QObject* object, QPluginLoader::staticInstances())
		{
			CacheBackend::Factory* factory(qobject_cast<CacheBackend::Factory*>(object));
			if(factory)
			{
				factories.append(factory);
			}
		}
		Q_ASSERT(!factories.isEmpty());
		m_backendFactory = factories.first();
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
