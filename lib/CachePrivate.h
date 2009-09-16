#pragma once

#include "Cache.h"
#include "CacheBackend.h"

namespace FastCgiQt
{
	class Cache::Private
	{
		public:
			Private(const QString& cacheName);
			~Private();

			CacheBackend* backend;
		private:
			void loadBackendFactory();

			static CacheBackend::Factory* m_backendFactory;
	};
};
