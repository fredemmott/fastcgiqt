#include "RequestQueue.h"

#include "Request.h"
#include "RequestRunner.h"

#include <QTimer>

namespace FastCgiQt
{
	RequestQueue::RequestQueue(SpawnerBase* spawner, const char* responderSlot, QObject* parent)
	: QObject(parent)
	, m_spawner(spawner)
	, m_responderSlot(responderSlot)
	, m_queueRunner(new QTimer(this))
	{
		m_queueRunner->setSingleShot(true);
		m_queueRunner->setInterval(0);
		connect(
			m_queueRunner,
			SIGNAL(timeout()),
			SLOT(flushQueue())
		);
	}

	void RequestQueue::respond(Request* request)
	{
		// Stop it being garbage collected
		request->setParent(this);
		// Queue it up
		m_unhandledRequests.enqueue(request);
		m_queueRunner->start();
	}

	void RequestQueue::flushQueue()
	{
		while(!m_unhandledRequests.isEmpty())
		{
			Request* request = m_unhandledRequests.dequeue();
			request->setParent(0);
			start(new RequestRunner(m_spawner, m_responderSlot, request));
		}
	}
}
