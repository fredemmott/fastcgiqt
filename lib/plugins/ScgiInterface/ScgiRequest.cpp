#include "ScgiRequest.h"

#include "InputDevice.h"
#include "ProxyOutputBackend.h"
#include "Request_Backend.h"

#include <QDebug>
#include <QTcpSocket>

namespace FastCgiQt
{
	ScgiRequest::~ScgiRequest()
	{
	}

	ScgiRequest::ScgiRequest(
		Responder::Generator generator,
		QTcpSocket* socket,
		QObject* parent
	)
	: CommunicationInterface::Worker(parent)
	, m_responderGenerator(generator)
	, m_inputDevice(0)
	, m_socket(socket)
	, m_request(new Request::Backend())
	, m_state(NewConnection)
	{
		socket->setParent(this);
		socket->open(QIODevice::ReadWrite);
		Q_ASSERT(socket->isOpen());
		Q_ASSERT(socket->isReadable());
		Q_ASSERT(socket->isWritable());

		connect(
			socket,
			SIGNAL(readyRead()),
			SLOT(readData())
		);
	}

	void ScgiRequest::readData()
	{
		switch(m_state)
		{
			case NewConnection:
				readHeaders();
				spawnResponder();
				break;
			case HaveHeaders:
				Q_ASSERT(m_inputDevice);
				m_inputDevice->appendData(m_socket->readAll());
				break;
		}
	}

	void ScgiRequest::cleanup(Responder* responder)
	{
		m_socket->close();
		delete responder;
		emit finished(thread());
		deleteLater();
	}

	void ScgiRequest::spawnResponder()
	{
		m_inputDevice = new InputDevice(this);

		Responder* responder = (*m_responderGenerator)(
			m_request,
			new OutputDevice(new ProxyOutputBackend(m_socket)),
			m_inputDevice,
			this
		);

		connect(
			responder,
			SIGNAL(finished(Responder*)),
			SLOT(cleanup(Responder*))
		);

		responder->start();
	}

	void ScgiRequest::readHeaders()
	{
		// Netstring: "length:blob,"
		QString lengthString;
		Q_FOREVER
		{
			char character;
			const bool readCharacter= m_socket->getChar(&character);
			Q_ASSERT(readCharacter);
			if(character == ':' || !readCharacter)
			{
				break;
			}
			else
			{
				lengthString.append(character);
			}
		}
		bool lengthIsNumeric;
		const unsigned int length = lengthString.toUInt(&lengthIsNumeric);
		Q_ASSERT(lengthIsNumeric);
		const QByteArray headerBlob = m_socket->read(length);
		Q_ASSERT(static_cast<qint64>(length) == headerBlob.length());
		m_socket->getChar(0); // skip ':' in netstring

		QList<QByteArray> parts = headerBlob.split(0);
		Q_ASSERT(parts.count() % 2); // null-terminated, last part is empty
		parts.takeLast();
		QHash<QString, QString> headers;
		while(!parts.isEmpty())
		{
			const QByteArray name = parts.takeFirst();
			const QByteArray value = parts.takeFirst();
			headers.insert(QString::fromLatin1(name), QString::fromLatin1(value));
		}
		Q_ASSERT(headers.contains("CONTENT_LENGTH"));
		Q_ASSERT(headers.contains("SCGI"));
		Q_ASSERT(headers.value("SCGI") == "1");
		m_request.backend()->addServerData(headers);
		m_state = HaveHeaders;
	}
};
