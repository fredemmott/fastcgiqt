#include "HttpRequest.h"

#include "InputDevice.h"
#include "Request_Backend.h"

#include <QDebug>
#include <QTimer>

// *MUST* be before libevent headers for TAILQ stuff
#include <sys/queue.h>

#include <event.h>
#include <evhttp.h>

namespace FastCgiQt
{
	HttpRequest::HttpRequest(Responder::Generator generator, struct evhttp_request* request, QObject* parent)
	: CommunicationInterface::Worker(parent)
	, m_generator(generator)
	, m_httpRequest(request)
	, m_request(new Request::Backend())
	{
		// Start from the event loop
		QTimer::singleShot(0, this, SLOT(start()));
	}

	void HttpRequest::start()
	{
		// 1. Read the headers
		QHash<QString, QString> headers;
		struct evkeyval* header;
		TAILQ_FOREACH(header, m_httpRequest->input_headers, next)
		{
			QString name = "HTTP_" + QString::fromLatin1(header->key).toUpper();
			name.replace('-', '_');
			headers.insert(name, QString::fromLatin1(header->value));
		}
		headers.insert("SERVER_SOFTWARE", "FastCgiQt/evhttp");
		headers.insert("GATEWAY_INTERFACE", "FastCgiQt/evhttp");
		qDebug() << "HEADERS:" << headers;
		// 2. Start the responder
	}

	HttpRequest::~HttpRequest()
	{
	}
};
