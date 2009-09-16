#include "CachePrivate.h"

#include "plugins/config.h"


#include <QCoreApplication>
#include <QDebug>
#include <QPluginLoader>
#include <QStringList>

namespace FastCgiQt
{
	CacheBackend::Factory* Cache::Private::m_backendFactory(NULL);

	Cache::Private::Private(const QString& cacheName)
	{
		if(!m_backendFactory)
		{
			loadBackendFactory();
		}
		backend = m_backendFactory->getCacheBackend(cacheName);
	}

	void Cache::Private::loadBackendFactory()
	{
		qDebug() << QCoreApplication::libraryPaths();
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

	Cache::Private::~Private()
	{
	}
};
