#include "Manager.h"

#include "DebugHandler.h"
#include "ManagerPrivate.h"

namespace FastCgiQt
{
	Manager::Manager(Responder::Generator responderGenerator, QObject* parent)
		:
			QObject(parent),
			d(new ManagerPrivate(responderGenerator, this))
	{
		new DebugHandler(this);
	}

	Manager::~Manager()
	{
	}
}
