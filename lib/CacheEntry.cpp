#include "CacheEntry.h"
#include "CacheEntryPrivate.h"

namespace FastCgiQt
{
	CacheEntry::CacheEntry(const QDateTime& timeStamp, const QByteArray& data)
	: d(new Private(timeStamp, data))
	{
	}

	void CacheEntry::setTimeStamp(const QDateTime& timeStamp)
	{
		d->timeStamp = timeStamp;
	}

	void CacheEntry::setData(const QByteArray& data)
	{
		d->data = data;
	}

	CacheEntry::CacheEntry()
	: d(new Private())
	{
	}

	CacheEntry::~CacheEntry()
	{
	}

	bool CacheEntry::isValid() const
	{
		return d->timeStamp.isValid() && !d->data.isNull();
	}

	QDateTime CacheEntry::timeStamp() const
	{
		return d->timeStamp;
	}

	QByteArray CacheEntry::data() const
	{
		return d->data;
	}

	CacheEntry::CacheEntry(const CacheEntry& other)
	: d(other.d)
	{
	}
}
