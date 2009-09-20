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
#include "Caches.h"

namespace FastCgiQt
{
	QAtomicInt Caches::m_referenceCount(0);
	QPointer<FileCache> Caches::m_fileCache;
	QPointer<FileDependentCache> Caches::m_requestCache;

	Caches::Caches()
	{
		m_referenceCount.ref();
		// Initialise caches
		fileCache();
		requestCache();
	}

	Caches::~Caches()
	{
		// cleanup caches
		if(!m_referenceCount.deref())
		{
			delete m_fileCache;
			delete m_requestCache;
		}
	}

	FileCache& Caches::fileCache()
	{
		if(!m_fileCache)
		{
			m_fileCache = new FileCache("FileCache");
		}
		return *m_fileCache;
	}

	FileDependentCache& Caches::requestCache()
	{
		if(!m_requestCache)
		{
			m_requestCache = new FileDependentCache("RequestCache");
		}
		return *m_requestCache;
	}
}
