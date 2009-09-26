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
#include "ScgiRequest.h"

#include "ProxyClientIODevice.h"

#include <QDebug>
#include <QTcpSocket>

namespace FastCgiQt
{
	ScgiRequest::~ScgiRequest()
	{
	}

	ScgiRequest::ScgiRequest(
		QTcpSocket* socket,
		QObject* parent
	)
	: CommunicationInterface::Worker(parent)
	, m_socket(socket)
	{
		socket->setParent(this);
		socket->open(QIODevice::ReadWrite);
		Q_ASSERT(socket->isOpen());
		Q_ASSERT(socket->isReadable());
		Q_ASSERT(socket->isWritable());

		connect(
			socket,
			SIGNAL(readyRead()),
			SLOT(startResponse())
		);
	}

	void ScgiRequest::cleanup()
	{
		m_socket->flush();
		m_socket->close();
		emit finished(thread());
		deleteLater();
	}

	void ScgiRequest::startResponse()
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
		ClientIODevice::HeaderMap headers;
		while(!parts.isEmpty())
		{
			const QByteArray name = parts.takeFirst();
			const QByteArray value = parts.takeFirst();
			headers.insert(name, value);
		}
		Q_ASSERT(headers.contains("CONTENT_LENGTH"));
		Q_ASSERT(headers.contains("SCGI"));
		Q_ASSERT(headers.value("SCGI") == "1");

		disconnect(m_socket, 0, this, 0);

		ProxyClientIODevice* device = new ProxyClientIODevice(headers, m_socket, this);
		connect(device, SIGNAL(destroyed(QObject*)), SLOT(cleanup()));
		emit newRequest(device);
	}
};
