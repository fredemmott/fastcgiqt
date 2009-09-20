#pragma once

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
