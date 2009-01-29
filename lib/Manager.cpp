#include "Manager.h"

#include "ManagerPrivate.h"

namespace FastCgiQt
{
	Manager::Manager(Responder::Generator responderGenerator, QObject* parent)
		:
			QObject(parent),
			d(new ManagerPrivate(responderGenerator, this))
	{
	}

	Manager::~Manager()
	{
	}
}
