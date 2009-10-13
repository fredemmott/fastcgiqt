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
