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
		Q_FOREACH(QObject* object, QPluginLoader::staticInstances())
		{
			CacheBackend::Factory* factory(qobject_cast<CacheBackend::Factory*>(object));
			if(factory && factory->loadSettings())
			{
				m_backendFactory = factory;
				break;
			}
		}
		Q_ASSERT(m_backendFactory);
	}

	Cache::~Cache()
	{
	}

	void Cache::remove(const QString& key)
	{
		m_backend->remove(key);
	}

	CacheEntry Cache::value(const QString& key) const
	{
		return m_backend->value(key);
	}

	void Cache::setValue(const QString& key, const CacheEntry& entry)
	{
		m_backend->setValue(key, entry);
	}

	QReadWriteLock* Cache::readWriteLock() const
	{
		return m_backend->readWriteLock();
	}
}
