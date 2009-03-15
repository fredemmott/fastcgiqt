#ifndef _FASTCGI_QT_CACHE_ENTRY_H
#define _FASTCGI_QT_CACHE_ENTRY_H

#include <QByteArray>
#include <QDateTime>

namespace FastCgiQt
{
	/**A cache entry composed of data and a timestamp.
	 *
	 * Cache entries can be invalid.
	 */
	class CacheEntry
	{
		public:
			/// Constructs a valid CacheEntry.
			CacheEntry(const QDateTime& timeStamp, const QByteArray& data);
			/// Constructs an invalid CacheEntry.
			CacheEntry();
			/// Whether or not the CacheEntry is valid.
			bool isValid() const;
			/// The time stamp associated with this CacheEntry.
			QDateTime timeStamp() const;
			/// The data associated with this CacheEntry.
			QByteArray data() const;
		private:
			bool m_isValid;
			QDateTime m_timeStamp;
			QByteArray m_data;
	};
};

#endif
