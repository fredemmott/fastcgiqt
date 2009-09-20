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
#include "Cache.h"
#include "CachePrivate.h"

namespace FastCgiQt
{
	Cache::Cache(const QString& cacheName)
	: d(new Private(cacheName))
	{
	}


	Cache::~Cache()
	{
	}

	void Cache::remove(const QString& key)
	{
		d->backend->remove(key);
	}

	CacheEntry Cache::value(const QString& key) const
	{
		return d->backend->value(key);
	}

	void Cache::setValue(const QString& key, const CacheEntry& entry)
	{
		d->backend->setValue(key, entry);
	}

	QReadWriteLock* Cache::readWriteLock() const
	{
		return d->backend->readWriteLock();
	}
}
