#include "CacheEntry.h"

namespace FastCgiQt
{
	CacheEntry::CacheEntry(const QDateTime& _timeStamp, const QByteArray& _data)
		:
			timeStamp(_timeStamp),
			data(_data)
	{
	}
}
