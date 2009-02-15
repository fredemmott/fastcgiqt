#ifndef _FASTCGI_QT_CACHE_H
#define _FASTCGI_QT_CACHE_H

#include <QByteArray>
#include <QCache>
#include <QDateTime>
#include <QString>

class QReadWriteLock;

namespace FastCgiQt
{
	struct CacheEntry
	{
		CacheEntry(const QDateTime& timeStamp, const QByteArray& data);
		const QDateTime timeStamp;
		const QByteArray data;
	};
	class Cache : private QCache<QString, CacheEntry>
	{
		public:
			Cache(int maxSize);
			~Cache();
			QReadWriteLock* readWriteLock() const;

			void clear();
			bool insert(const QString& key, CacheEntry* object);
			bool contains(const QString& key) const;
			bool remove(const QString& key);
			int maxCost() const;
			void setMaxCost(int cost);
			CacheEntry* operator[](const QString& key) const;
		private:
			QReadWriteLock* m_lock;
	};
}

#endif
