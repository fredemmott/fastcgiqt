#ifndef _FASTCGI_QT_RAM_CACHE_H
#define _FASTCGI_QT_RAM_CACHE_H

#include "CacheBackend.h"

#include <QCache>
#include <QHash>
#include <QObject>
#include <QReadWriteLock>

namespace FastCgiQt
{
	class RamCache : public CacheBackend
	{
		public:
			RamCache(const QString& cacheName);
			virtual void remove(const QString& key);
			virtual CacheEntry value(const QString& key) const;
			virtual void setValue(const QString& key, const CacheEntry& entry);
			virtual QReadWriteLock* readWriteLock() const;
		private:
			const QString m_keyPrefix;
			static QCache<QString, CacheEntry> m_cache;
			static QReadWriteLock m_lock;
	};

	class RamCacheFactory : public QObject, public CacheBackend::Factory
	{
		Q_OBJECT
		Q_INTERFACES(FastCgiQt::CacheBackend::Factory)
		public:
			virtual CacheBackend* getCacheBackend(const QString& cacheName) const;
	};
}

#endif
