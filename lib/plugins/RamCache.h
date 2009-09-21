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
#ifndef _FASTCGI_QT_RAM_CACHE_H
#define _FASTCGI_QT_RAM_CACHE_H

#include "CacheBackend.h"

#include <QCache>
#include <QHash>
#include <QObject>
#include <QReadWriteLock>

namespace FastCgiQt
{
	///@internal @brief QCache-based CacheBackend implemntation.
	class RamCache : public CacheBackend
	{
		public:
			RamCache(const QString& cacheName);
			virtual void remove(const QString& key);
			virtual CacheEntry value(const QString& key) const;
			virtual void setValue(const QString& key, const CacheEntry& entry);
			virtual QReadWriteLock* readWriteLock() const;
			static void setMaxSize(int size);
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
			virtual bool loadSettings();
			virtual CacheBackend* createCacheBackend(const QString& cacheName) const;
	};
}

#endif
