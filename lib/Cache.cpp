#include "Cache.h"

#include <QDebug>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QThread>
#include <QWriteLocker>

typedef QCache<QString, FastCgiQt::CacheEntry> super;

namespace FastCgiQt
{
	CacheEntry::CacheEntry(const QDateTime& _timeStamp, const QByteArray& _data)
		:
			timeStamp(_timeStamp),
			data(_data)
	{
	}

	Cache::Cache(int maxSize)
		:
			super(maxSize),
			m_lock(new QReadWriteLock(QReadWriteLock::Recursive))
	{
	}

	Cache::~Cache()
	{
		delete m_lock;
	}

	bool Cache::insert(const QString& key, CacheEntry* object)
	{
		QWriteLocker lock(m_lock);
		return super::insert(key, object, object->data.length());
	}
	
	bool Cache::contains(const QString& key) const
	{
		QReadLocker lock(m_lock);
		return super::contains(key);
	}

	bool Cache::remove(const QString& key)
	{
		QWriteLocker lock(m_lock);
		return super::remove(key);
	}

	QReadWriteLock* Cache::readWriteLock() const
	{
		return m_lock;
	}

	CacheEntry* Cache::operator[](const QString& key) const
	{
		QReadLocker lock(m_lock);
		return super::operator[](key);
	}

	void Cache::clear()
	{
		QWriteLocker lock(m_lock);
		super::clear();
	}

	int Cache::maxCost() const
	{
		QReadLocker lock(m_lock);
		return super::maxCost();
	}

	void Cache::setMaxCost(int cost)
	{
		QWriteLocker lock(m_lock);
		super::setMaxCost(cost);
	}
}
