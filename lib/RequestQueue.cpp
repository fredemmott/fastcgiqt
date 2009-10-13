/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
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
		respond(m_spawner, m_responderSlot, request);
	}

	void RequestQueue::respond(SpawnerBase* spawner, const char* responderSlot, Request* request)
	{
		// Stop it being garbage collected
		request->setParent(this);
		// Queue it up
		m_unhandledRequests.enqueue(new RequestRunner(spawner, responderSlot, request));
		m_queueRunner->start();
	}

	void RequestQueue::flushQueue()
	{
		while(!m_unhandledRequests.isEmpty())
		{
			RequestRunner* runner = m_unhandledRequests.dequeue();
			runner->request()->setParent(0);
			start(runner);
		}
	}
}
