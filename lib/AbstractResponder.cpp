#include "AbstractResponder.h"

namespace FastCgiQt
{
	SpawnerBase::SpawnerBase()
	{
	}

	SpawnerBase::~SpawnerBase()
	{
	}

	AbstractResponder::AbstractResponder(QObject* parent)
	: QObject(parent)
	{
	}
	
	AbstractResponder::~AbstractResponder()
	{
	}
};
