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
#include "HttpInterface.h"
#include "HttpDaemon.h"
#include "HttpRequest.h"

#include "Settings.h"

#include <QDebug>
#include <QStringList>

namespace FastCgiQt
{
	HttpInterface::HttpInterface(Responder::Generator responderGenerator, QObject* parent)
	: CommunicationInterface(parent)
	, m_responderGenerator(responderGenerator)
	{
	}

	QStringList HttpInterface::backends() const
	{
		return QStringList() << "HTTP";
	}

	void HttpInterface::configureHttpd(const QString& backend)
	{
		QTextStream cin(stdin);
		QTextStream cout(stdout);
		Settings settings;
		settings.beginGroup("FastCGI");

		if(backend.toUpper() == "HTTP")
		{
			QString portString;
			cout << "Port number: " << flush;
			portString = cin.readLine();
			bool ok;
			quint32 portNumber = portString.toUInt(&ok);
			if(!(ok && portNumber))
			{
				qFatal("Not a valid port number.");
				return;
			}
			settings.setValue("portNumber", portNumber);
			return;
		}
		qFatal("Unknown HTTP backend: %s", qPrintable(backend));
	}

	bool HttpInterface::startBackend(const QString& backend)
	{
		if(backend != "HTTP")
		{
			return false;
		}
		const quint16 portNumber = Settings().value("FastCGI/portNumber", 0).value<quint16>();
		if(portNumber == 0)
		{
			return false;
		}

		HttpDaemon* daemon = new HttpDaemon(portNumber, this);
		connect(
			daemon,
			SIGNAL(spawnRequest(struct evhttp_request*)),
			SLOT(spawnRequest(struct evhttp_request*))
		);
		daemon->start();

		return true;
	}

	HttpInterface::~HttpInterface()
	{
	}

	void HttpInterface::spawnRequest(struct evhttp_request* request)
	{
		addWorker(new HttpRequest(m_responderGenerator, request, NULL));
	}

	bool HttpInterface::isFinished() const
	{
		// "isFinished" means "thread load is zero; are you going to get any requests?"; answer is 'maybe'
		return false;
	}
}
