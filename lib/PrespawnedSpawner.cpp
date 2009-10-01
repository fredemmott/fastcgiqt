#include "PrespawnedSpawner.h"

namespace FastCgiQt
{
	PrespawnedSpawner::PrespawnedSpawner(QObject* object)
	: m_object(object)
	{
		Q_ASSERT(!object->parent());
	}

	QObject* PrespawnedSpawner::spawn(QObject* parent) const
	{
		Q_UNUSED(parent);
		return m_object;
	}

	bool PrespawnedSpawner::persist() const
	{
		return true;
	}
};
