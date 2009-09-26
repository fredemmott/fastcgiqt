#include "RequestWorker.h"

#include "InputDevice.h"
#include "ProxyOutputBackend.h"
#include "Request_Backend.h"

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

		Responder* responder = (*m_generator)(
			m_request,
			new OutputDevice(new ProxyOutputBackend(m_device)),
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
