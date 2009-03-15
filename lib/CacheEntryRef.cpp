#include "CacheEntryRef.h"

namespace FastCgiQt
{
	CacheEntryRef::CacheEntryRef(
		const QString& key,
		const CacheEntry& entry,
		Observer* observer
	)
		:
			m_modified(false),
			m_key(key),
			m_entry(entry),
			m_observer(observer)
	{
	}

	CacheEntryRef::~CacheEntryRef()
	{
		if(m_modified && m_observer)
		{
			m_observer->cacheEntryRefModified(m_key, m_entry);
		}
	}

	QString CacheEntryRef::key() const
	{
		return m_key;
	}

	QDateTime CacheEntryRef::timeStamp() const
	{
		return m_entry.timeStamp();
	}

	QByteArray CacheEntryRef::data() const
	{
		return m_entry.data();
	}

	CacheEntryRef& CacheEntryRef::operator=(const CacheEntry& entry)
	{
		m_modified = true;
		m_entry = entry;
		return *this;
	}
}
