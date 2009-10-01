#pragma once

#include "Responder.h"

namespace FastCgiQt
{
	class PrespawnedSpawner : public SpawnerBase
	{
		public:
			PrespawnedSpawner(QObject* object);
			QObject* spawn(QObject* parent) const;
			bool persist() const;
		private:
			QObject* m_object;
	};
};
