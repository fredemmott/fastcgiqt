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
#include "CommunicationInterface.h"
#include "CommunicationInterface_Worker.h"
#include "RequestFactory.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

namespace FastCgiQt
{
	CommunicationInterface::CommunicationInterface(QObject* parent)
	: QObject(parent)
	{
	}

	CommunicationInterface::~CommunicationInterface()
	{
	}

	bool CommunicationInterface::start(const QString& backend)
	{
		return startBackend(backend);
	}

	void CommunicationInterface::configureHttpd(const QString& backend)
	{
		Q_UNUSED(backend);
	}

	void CommunicationInterface::addWorker(Worker* worker)
	{
		connect(
			worker,
			SIGNAL(newRequest(ClientIODevice*)),
			SLOT(addRequest(ClientIODevice*))
		);
	}

	QStringList CommunicationInterface::detectedBackends() const
	{
		return QStringList();
	}

	bool CommunicationInterface::requiresConfiguration(const QString& backend) const
	{
		Q_UNUSED(backend);
		return true;
	}

	void CommunicationInterface::addRequest(ClientIODevice* device)
	{
		Request* request = RequestFactory::createRequest(device, 0);
		emit newRequest(request);
		if(!request->parent())
		{
			delete request;
		}
	}
};
