#ifndef _FASTCGI_QT_CACHE_ENTRY_H
#define _FASTCGI_QT_CACHE_ENTRY_H

#include <QByteArray>
#include <QDateTime>

namespace FastCgiQt
{
	///@internal @brief A cache entry composed of data and a timestamp.
	struct CacheEntry
	{
		CacheEntry(const QDateTime& timeStamp, const QByteArray& data);
		const QDateTime timeStamp;
		const QByteArray data;
	};
};

#endif
