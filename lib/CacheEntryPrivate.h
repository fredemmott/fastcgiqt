#pragma once
#include "CacheEntry.h"

namespace FastCgiQt
{
	class CacheEntry::Private : public QSharedData
	{
		public:
			Private();
			Private(const QDateTime& timeStamp, const QByteArray& data);
			Private(const Private& other);
			~Private();

			QDateTime timeStamp;
			QByteArray data;
	};
};
