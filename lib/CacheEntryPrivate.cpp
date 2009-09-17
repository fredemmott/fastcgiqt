#include "CacheEntryPrivate.h"

namespace FastCgiQt
{
	CacheEntry::Private::Private()
	: QSharedData()
	{
	}

	CacheEntry::Private::Private(const QDateTime& timeStamp, const QByteArray& data)
	{
		this->timeStamp = timeStamp;
		this->data = data;
	}

	CacheEntry::Private::Private(const Private& other)
	: QSharedData(other)
	, timeStamp(other.timeStamp)
	, data(other.data)
	{
	}

	CacheEntry::Private::~Private()
	{
	}
};
