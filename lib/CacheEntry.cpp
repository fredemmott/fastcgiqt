#include "CacheEntry.h"

namespace FastCgiQt
{
	CacheEntry::CacheEntry(const QDateTime& timeStamp, const QByteArray& data)
	: m_timeStamp(timeStamp)
	, m_data(data)
	{
	}

	void CacheEntry::setTimeStamp(const QDateTime& timeStamp)
	{
		m_timeStamp = timeStamp;
	}

	void CacheEntry::setData(const QByteArray& data)
	{
		m_data = data;
	}

	CacheEntry::CacheEntry()
	{
	}

	bool CacheEntry::isValid() const
	{
		return m_timeStamp.isValid() && !m_data.isNull();
	}

	QDateTime CacheEntry::timeStamp() const
	{
		return m_timeStamp;
	}

	QByteArray CacheEntry::data() const
	{
		return m_data;
	}
}
