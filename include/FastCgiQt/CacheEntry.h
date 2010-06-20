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
#ifndef _FASTCGIQT_CACHEENTRY
#define _FASTCGIQT_CACHEENTRY

#include <QByteArray>
#include <QDateTime>
#include <QSharedDataPointer>

namespace FastCgiQt
{
	/**A cache entry composed of data and a timestamp.
	 *
	 * Cache entries can be invalid.
	 *
	 * @ingroup plugins
	 */
	class CacheEntry
	{
		public:
			~CacheEntry();
			/// Construct a valid CacheEntry with the current date and time
			CacheEntry(const QByteArray& data);
			/// Constructs a valid CacheEntry.
			CacheEntry(const QDateTime& timeStamp, const QByteArray& data);
			/// Constructs an invalid CacheEntry.
			CacheEntry();
			/// Copy-construct a cache entry
			CacheEntry(const CacheEntry& other);
			/// Whether or not the CacheEntry is valid.
			bool isValid() const;
			/// The time stamp associated with this CacheEntry.
			QDateTime timeStamp() const;
			/// The data associated with this CacheEntry.
			QByteArray data() const;

			void operator=(const CacheEntry& other);
		protected:
			void setTimeStamp(const QDateTime&);
			void setData(const QByteArray&);
		private:
			class Private;
			QSharedDataPointer<Private> d;
	};
};
#endif // _FASTCGIQT_CACHEENTRY
