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
		backend = m_backendFactory->createCacheBackend(cacheName);
	}

	void Cache::Private::loadBackendFactory()
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

	Cache::Private::~Private()
	{
	}
};
