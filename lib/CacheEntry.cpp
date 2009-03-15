#include "CacheEntry.h"

namespace FastCgiQt
{
	CacheEntry::CacheEntry(const QDateTime& timeStamp, const QByteArray& data)
		:
			m_isValid(true),
			m_timeStamp(timeStamp),
			m_data(data)
	{
	}

	CacheEntry::CacheEntry()
		:
			m_isValid(false)

	{
	}

	bool CacheEntry::isValid() const
	{
		return m_isValid;
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
