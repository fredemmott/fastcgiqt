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
#include "RamCache.h"
#include "Settings.h"

#include <QCoreApplication>
#include <QDebug>
#include <QReadLocker>
#include <QStringList>
#include <QWriteLocker>

namespace FastCgiQt
{
	QCache<QString, CacheEntry> RamCache::m_cache(0);
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

	bool RamCacheFactory::loadSettings()
	{
		Settings settings;
		if(settings.value("cache/backend", "RamCache") == "RamCache")
		{
			RamCache::setMaxSize(settings.value("RamCache/maxSize", 10 * 1024 * 1024).toInt());
			return true;
		}
		return false;
	}

	void RamCache::setMaxSize(int size)
	{
		m_cache.setMaxCost(size);
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

Q_EXPORT_PLUGIN2(FastCgiQt_RamCache, FastCgiQt::RamCacheFactory)
