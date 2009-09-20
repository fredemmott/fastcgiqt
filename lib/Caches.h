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
#ifndef _FASTCGIQT_CACHES_H
#define _FASTCGIQT_CACHES_H

#include "FileCache.h"

#include <QAtomicInt>
#include <QPointer>

namespace FastCgiQt
{
	/** @internal @brief Class providing access to global file/request caches.
	 *
	 * An instance of this class is used for reference counting and cleanup.
	 */
	class Caches
	{
		public:
			/// Increase the reference count.
			Caches();
			/// Decrease the reference count.
			~Caches();
			/// Get a reference to the file cache.
			static FileCache& fileCache();
			/// Get a reference to the reference cache.
			static FileDependentCache& requestCache();
		private:
			static QAtomicInt m_referenceCount;
			static QPointer<FileCache> m_fileCache;
			static QPointer<FileDependentCache> m_requestCache;
	};
}

#endif
