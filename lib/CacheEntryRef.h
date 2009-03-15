#ifndef _FASTCGI_QT_CACHE_ENTRY_REF_H
#define _FASTCGI_QT_CACHE_ENTRY_REF_H

#include "CacheEntry.h"

namespace FastCgiQt
{
	class CacheEntryRef
	{
		public:
			class Observer
			{
				public:
					virtual void cacheEntryRefModified(const QString& key, const CacheEntry& entry) = 0;
			};

			CacheEntryRef(const QString& key, const CacheEntry& entry, Observer* observer);
			~CacheEntryRef();

			QString key() const;
			QDateTime timeStamp() const;
			QByteArray data() const;

			CacheEntryRef& operator=(const CacheEntry&);
		private:
			bool m_modified;
			QString m_key;
			CacheEntry m_entry;
			Observer* m_observer;
	};
}

#endif
