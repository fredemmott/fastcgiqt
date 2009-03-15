#ifndef _FASTCGI_QT_CACHE_H
#define _FASTCGI_QT_CACHE_H

#include "CacheEntry.h"

#include <QByteArray>
#include <QCache>
#include <QDateTime>
#include <QSet>
#include <QString>

class QReadWriteLock;

namespace FastCgiQt
{
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
			void clear();
			bool insert(const QString& key, CacheEntry* object);
			bool contains(const QString& key) const;
			bool remove(const QString& key);
			int totalCost() const;
			int maxCost() const;
			int count() const;
			int size() const;
			void setMaxCost(int cost);
			CacheEntry* operator[](const QString& key) const;
		private:
			QReadWriteLock* m_lock;
	};
}

#endif
