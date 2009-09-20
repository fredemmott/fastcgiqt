/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include "CacheEntry.h"
#include "CacheEntryPrivate.h"

namespace FastCgiQt
{
	CacheEntry::CacheEntry(const QDateTime& timeStamp, const QByteArray& data)
	: d(new Private(timeStamp, data))
	{
	}

	CacheEntry::CacheEntry(const QByteArray& data)
	: d(new Private(QDateTime::currentDateTime().toUTC(), data))
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

	void CacheEntry::operator=(const CacheEntry& other)
	{
		d = other.d;
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
