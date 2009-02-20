#ifndef _FASTCGI_QT_CACHE_H
#define _FASTCGI_QT_CACHE_H

#include <QByteArray>
#include <QCache>
#include <QDateTime>
#include <QSet>
#include <QString>

class QReadWriteLock;

namespace FastCgiQt
{
	///@internal @brief A cache entry composed of data and a timestamp.
	struct CacheEntry
	{
		CacheEntry(const QDateTime& timeStamp, const QByteArray& data);
		const QDateTime timeStamp;
		const QByteArray data;
	};
	/** @internal @brief A read-write-locked cache structure.
	 *
	 * This is a QCache<QString, CacheEntry> with a recursive QReadWriteLock.
	 *
	 * All members lock as neccessary, but you may need to lock at an outer
	 * scope depending on the usage.
	 *
	 * Keep in mind that even with a recursive lock, the locks must be of
	 * the same type; for example, all the writing calls will deadlock
	 * if called while a read lock is open.
	 */
	class Cache : private QCache<QString, CacheEntry>
	{
		public:
			Cache(int maxSize);
			~Cache();
			/// Return a pointer to the QReadWriteLock.
			QReadWriteLock* readWriteLock() const;

			QList<QString> keys() const;
			virtual void clear();
			virtual bool insert(const QString& key, CacheEntry* object);
			bool contains(const QString& key) const;
			virtual bool remove(const QString& key);
			int totalCost() const;
			int maxCost() const;
			int count() const;
			int size() const;
			void setMaxCost(int cost);
			CacheEntry* operator[](const QString& key) const;

			class Observer
			{
				public:
					virtual void entryAdded(const QString& key, CacheEntry* entry) = 0;
			};
			void addObserver(Observer* observer);
		private:
			QReadWriteLock* m_lock;
			QSet<Observer*> m_observers;
	};
}

#endif
