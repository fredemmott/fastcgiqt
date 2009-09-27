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
#include "RequestWorker.h"

#include "InputDevice.h"
#include "OutputDevice.h"
#include "Request_Backend.h"

#include <QDebug>
#include <QTimer>

namespace FastCgiQt
{
	RequestWorker::RequestWorker(ClientIODevice* device, Responder::Generator generator, QObject* parent)
	: Worker(parent)
	, m_device(device)
	, m_generator(generator)
	, m_inputDevice(0)
	, m_request(new Request::Backend())
	{
		QTimer::singleShot(0, this, SLOT(start()));
	}

	void RequestWorker::copyPostData()
	{
		const QByteArray data = m_device->readAll();
		// eww
		m_inputDevice->appendData(data);
		m_request.backend()->appendContent(data);
	}

	RequestWorker::~RequestWorker()
	{
	}

	void RequestWorker::start()
	{
		connect(
			m_device,
			SIGNAL(readyRead()),
			SLOT(copyPostData())
		);
		m_inputDevice = new InputDevice(this);
		// FIXME
		// - Request::Backend expects QString/QString headers
		// - ClientIODevice expects QByteArray/QByteArray headers
		//
		// Want to switch to QByteArray, but API compatibility?
		const ClientIODevice::HeaderMap byteHeaders = m_device->requestHeaders();
		QHash<QString, QString> stringHeaders;
		for(
			ClientIODevice::HeaderMap::ConstIterator it = byteHeaders.constBegin();
			it != byteHeaders.constEnd();
			++it
		)
		{
			stringHeaders.insertMulti(
				QString::fromUtf8(it.key()),
				QString::fromUtf8(it.value())
			);
		}
		m_request.backend()->addServerData(stringHeaders);
		// Must be done after the server data is added, otherwise confusion happens due to
		// CONTENT_LENGTH header not being set.
		copyPostData();

		Responder* responder = (*m_generator)(
			m_request,
			new OutputDevice(m_device),
			m_inputDevice,
			this
		);

		connect(
			responder,
			SIGNAL(finished(Responder*)),
			SLOT(cleanupResponder(Responder*))
		);

		responder->start();
	}

	void RequestWorker::cleanupResponder(Responder* responder)
	{
		delete responder;
		delete m_device;
		emit finished(thread());
		deleteLater();
	}
};
