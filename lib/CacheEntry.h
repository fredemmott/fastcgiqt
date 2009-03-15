#ifndef _FASTCGI_QT_CACHE_ENTRY_H
#define _FASTCGI_QT_CACHE_ENTRY_H

#include <QByteArray>
#include <QDateTime>

namespace FastCgiQt
{
	///@internal @brief A cache entry composed of data and a timestamp.
	class CacheEntry
	{
		public:
			CacheEntry(const QDateTime& timeStamp, const QByteArray& data);
			QDateTime timeStamp() const;
			QByteArray data() const;
		private:
			QDateTime m_timeStamp;
			QByteArray m_data;
	};
};

#endif
