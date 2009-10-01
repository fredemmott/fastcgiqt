#include "RequestRunner.h"

#include "Request.h"

#include <QDebug>
#include <QMetaMethod>
#include <QMetaObject>
#include <QThread>

namespace FastCgiQt
{
	RequestRunner::RequestRunner(SpawnerBase* spawner, const char* responderSlot, Request* request)
	: m_spawner(spawner)
	, m_slot(responderSlot)
	, m_request(request)
	{
		Q_ASSERT(QMetaObject::checkConnectArgs(SIGNAL(foo(FastCgiQt::Request*)), responderSlot));
		++m_slot; // skip QSLOT_CODE
	}

	RequestRunner::~RequestRunner()
	{
	}

	Request* RequestRunner::request() const
	{
		return m_request;
	}

	void RequestRunner::run()
	{
		Q_ASSERT(!m_request->parent());

		QObject* responder = m_spawner->spawn(0);
		const QMetaObject* metaObject = responder->metaObject();
		const int index = metaObject->indexOfSlot(m_slot);
		Q_ASSERT(index != -1);

		metaObject->method(index).invoke(responder, Qt::DirectConnection, Q_ARG(FastCgiQt::Request*, m_request));

		if(!m_request->parent())
		{
			delete m_request;
			if(!m_spawner->persist())
			{
				delete responder;
			}
		}
	}
};
