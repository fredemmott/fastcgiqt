#ifndef _FASTCGI_QT_RAM_CACHE_H
#define _FASTCGI_QT_RAM_CACHE_H

#include "CacheBackend.h"

#include <QCache>
#include <QHash>
#include <QReadWriteLock>

namespace FastCgiQt
{
	class RamCache : public CacheBackend
	{
		public:
			virtual void remove(const QString& key);
			virtual CacheEntry value(const QString& key) const;
			virtual void setValue(const QString& key, const CacheEntry& entry);
			virtual QReadWriteLock* readWriteLock() const;
		private:
			static QCache<QString, CacheEntry> m_cache;
			static QReadWriteLock m_lock;
	};

	class RamCacheFactory : public CacheBackend::Factory
	{
		public:
			virtual CacheBackend* getCacheBackend() const;
	};
}

#endif
