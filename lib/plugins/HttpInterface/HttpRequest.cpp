#include "HttpRequest.h"

// *MUST* be before libevent headers for TAILQ stuff
#include "sys_queue.h"

#include "HttpOutputBackend.h"
#include "InputDevice.h"
#include "Request_Backend.h"

#include <QDebug>
#include <QTimer>

#include <event.h>
#include <evhttp.h>

namespace FastCgiQt
{
	HttpRequest::HttpRequest(Responder::Generator generator, struct evhttp_request* request, QObject* parent)
	: CommunicationInterface::Worker(parent)
	, m_generator(generator)
	, m_httpRequest(request)
	, m_inputDevice(0)
	, m_request(new Request::Backend())
	{
		// Start from the event loop
		QTimer::singleShot(0, this, SLOT(start()));
	}

	void HttpRequest::start()
	{
		// 1. Read the headers
		QHash<QString, QString> headers;
		// Read the HTTP headers
		const size_t contentLength = EVBUFFER_LENGTH(m_httpRequest->input_buffer);
		headers.insert("CONTENT_LENGTH", QString::number(contentLength));

		struct evkeyval* header;
		TAILQ_FOREACH(header, m_httpRequest->input_headers, next)
		{
			QString name = QString::fromLatin1(header->key).toUpper();
			name.replace('-', '_');
			const QString value(QString::fromLatin1(header->value));
			if(name == "CONTENT_LENGTH")
			{
				Q_ASSERT(value == QString::number(contentLength));
			}
			else if(name == "CONTENT_TYPE")
			{
				headers.insert(name, value);
			}
			else
			{
				headers.insert("HTTP_" + name, value);
			}
		}

		// Add special CGI headers
		headers.insert("SERVER_SOFTWARE", "FastCgiQt/evhttp");
		headers.insert("GATEWAY_INTERFACE", "CGI/1.1");
		switch(m_httpRequest->type)
		{
			case EVHTTP_REQ_GET:
				headers.insert("REQUEST_METHOD", "GET");
				break;
			case EVHTTP_REQ_POST:
				headers.insert("REQUEST_METHOD", "POST");
				break;
			case EVHTTP_REQ_HEAD:
				headers.insert("REQUEST_METHOD", "HEAD");
				break;
		}
		headers.insert("REMOTE_ADDR", QLatin1String(m_httpRequest->remote_host));
		headers.insert("REMOTE_PORT", QString::number(m_httpRequest->remote_port));
		{
			const QByteArray uri(m_httpRequest->uri);
			headers.insert("REQUEST_URI", uri);
			const int queryStringPosition = uri.indexOf('?');
			if(queryStringPosition == -1)
			{
				headers.insert("QUERY_STRING", QString());
			}
			else
			{
				headers.insert("QUERY_STRING", uri.mid(queryStringPosition + 1));
			}
		}
		headers.insert("SERVER_PROTOCOL", QString("HTTP/%1.%2").arg(static_cast<int>(m_httpRequest->major)).arg(static_cast<int>(m_httpRequest->minor)));

		m_request.backend()->addServerData(headers);
		// 2. Read POST data
		m_inputDevice = new InputDevice(this);
		const QByteArray postData(reinterpret_cast<const char*>(EVBUFFER_DATA(m_httpRequest->input_buffer)), contentLength);
		m_request.backend()->appendContent(postData); // for easy access
		m_inputDevice->appendData(postData);

		// 3. Start the responder
		Responder* responder = (*m_generator)(
			m_request,
			new OutputDevice(new HttpOutputBackend(m_httpRequest), this),
			m_inputDevice,
			this
		);
		connect(
			responder,
			SIGNAL(finished(Responder*)),
			SLOT(cleanup(Responder*))
		);
		qDebug() << Q_FUNC_INFO << "Calling start";
		responder->start();
	}

	void HttpRequest::cleanup(Responder* responder)
	{
		qDebug() << Q_FUNC_INFO;
		delete responder;
		m_httpRequest = 0;
		emit finished(thread());
		deleteLater();
	}

	HttpRequest::~HttpRequest()
	{
		qDebug() << Q_FUNC_INFO;
	}
};
