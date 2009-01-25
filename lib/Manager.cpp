#include "Manager.h"

#include "ManagerPrivate.h"

namespace FastCgiQt
{
	Manager::Manager(ResponderGenerator responderGenerator, QObject* parent)
		:
			QObject(parent),
			d(new ManagerPrivate(responderGenerator, this))
	{
	}

	Manager::~Manager()
	{
	}
}
