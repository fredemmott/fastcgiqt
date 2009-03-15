#include "CacheEntry.h"

namespace FastCgiQt
{
	CacheEntry::CacheEntry(const QDateTime& timeStamp, const QByteArray& data)
		:
			m_timeStamp(timeStamp),
			m_data(data)
	{
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
