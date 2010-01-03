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
#include "Request.h"
#include "Request_Backend.h"

#include <QCoreApplication>

#include <QDebug>
#include <QNetworkCookie>
#include <QRegExp>
#include <QUrl>

namespace FastCgiQt
{
	QString Request::fullUri() const
	{
		// not nice, but better than the old code and backwards-compatible. Nicer approach in BigChanges branch.
		return QUrl::fromPercentEncoding(serverData("REQUEST_URI").toLatin1());
	}

	QUrl Request::url() const
	{
		///@fixme HTTPS, alternative ports
		return QUrl::fromPercentEncoding(QString("http://%1%2").arg(serverData("HTTP_HOST")).arg(serverData("REQUEST_URI")).toLatin1());
	}

	Request::Request()
	: m_backend(0)
	{
	}

	Request::Request(Backend* backend)
	: m_backend(backend)
	{
		if(m_backend)
		{
			m_backend->setRequest(this);
		}
	}

	QString Request::contentType() const
	{
		return m_backend->contentType();
	}

	quint64 Request::contentLength() const
	{
		return m_backend->contentLength();
	}

	void Request::waitForAllContent() const
	{
		while(!haveAllContent())
		{
			QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
		}
	}

	QByteArray Request::content() const
	{
		return m_backend->content();
	}

	bool Request::haveAllContent() const
	{
		return m_backend->haveAllContent();
	}

	bool Request::isValid() const
	{
		return m_backend;
	}

	QList<QNetworkCookie> Request::cookies() const
	{
		const QHash<QString, QString> serverData = m_backend->serverData();
		QList<QNetworkCookie> cookies;
		for(QHash<QString, QString>::ConstIterator it = serverData.constBegin(); it != serverData.constEnd(); ++it)
		{
			if(it.key().toUpper() == "HTTP_COOKIE")
			{
				cookies.append(QNetworkCookie::parseCookies(it.value().toLatin1()));
			}
		}
		return cookies;
	}

	QString Request::serverData(const QString& name) const
	{
		return serverData().value(name);
	}

	QHash<QString, QString> Request::serverData() const
	{
		return m_backend->serverData();
	}

	QString Request::getData(const QString& name) const
	{
		return getData().value(name);
	}

	QHash<QString, QString> Request::getData() const
	{
		return m_backend->getData();
	}

	QString Request::postData(const QString& name) const
	{
		return postData().value(name);
	}

	QHash<QString, QString> Request::postData() const
	{
		return m_backend->postData();
	}

	QString Request::baseUri() const
	{
		const int queryStringLength = serverData("QUERY_STRING").length();
		const int pathInfoLength = QUrl::toPercentEncoding(serverData("PATH_INFO")).length();
		QString baseUri = serverData("REQUEST_URI");
		baseUri.chop(queryStringLength + 1 + pathInfoLength);
		return baseUri;
	}

	Request::~Request()
	{
	}

	void Request::operator=(const Request& other)
	{
		m_backend = other.backend();
	}

	Request::Request(const Request& other)
	: m_backend(other.backend())
	{
	}

	QSharedPointer<Request::Backend> Request::backend() const
	{
		return m_backend;
	}
}
