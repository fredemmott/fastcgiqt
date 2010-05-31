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
#include "Request_Private.h"

#include <QDebug>
#include <QNetworkCookie>
#include <QRegExp>
#include <QUrl>

namespace FastCgiQt
{
	Request::Request(Private* _d, QObject* parent)
	: QIODevice(parent)
	, d(_d)
	{
		open(QIODevice::ReadWrite);
		d->device->setParent(this);
		connect(
			d->device,
			SIGNAL(readyRead()),
			this,
			SIGNAL(readyRead())
		);
	}

	bool Request::atEnd() const
	{
		const bool atEnd = d->device->atEnd() && QIODevice::atEnd();
		return atEnd;
	}

	bool Request::isSequential() const
	{
		return true;
	}

	qint64 Request::readData(char* data, qint64 maxSize)
	{
		Q_ASSERT(d->postDataMode == Private::UnknownPostData || d->postDataMode == Private::RawPostData);
		d->postDataMode = Private::RawPostData;
		Q_ASSERT(d->device->isOpen());
		Q_ASSERT(d->device->isReadable());
		const qint64 bytesRead = d->device->read(data, maxSize);
		return bytesRead;
	}

	qint64 Request::writeData(const char* data, qint64 maxSize)
	{
		QIODevice* device = d->device;

		if(!d->haveSentHeaders)
		{
			d->haveSentHeaders = true;
			for(ClientIODevice::HeaderMap::ConstIterator it = d->responseHeaders.constBegin(); it != d->responseHeaders.constEnd(); ++it)
			{
				device->write(it.key());
				device->write(": ");
				device->write(it.value());
				device->write("\r\n");
			}
			device->write("\r\n");
		}
		return device->write(data, maxSize);
	}

	qint64 Request::size() const
	{
		return QString::fromLatin1(rawValue(ServerData, "CONTENT_LENGTH")).toLongLong();
	}

	QUrl Request::url(UrlPart part) const
	{
		QUrl url;
		// Protocol and host are needed, regardless of part
		if(rawValue(ServerData,"HTTPS") == "on")
		{
			url.setScheme("https");
		}
		else
		{
			url.setScheme("http");
		}
		// authority == user:password@host:port - as HTTP_HOST contains user and port, go with that
		url.setAuthority(value(ServerData, "HTTP_HOST"));

		const int queryStringOffset = rawValue(ServerData, "REQUEST_URI").contains('?') ? 1 : 0;
		const int queryStringLength = rawValue(ServerData, "QUERY_STRING").length() + queryStringOffset;
		switch(part)
		{
			case RootUrl:
			{
				const int pathInfoLength = rawValue(ServerData, "PATH_INFO").length();
				QByteArray basePath = rawValue(ServerData, "REQUEST_URI");
				basePath.chop(queryStringLength + pathInfoLength);
				url.setEncodedPath(basePath);
				break;
			}
			case LocationUrl:
			case RequestUrl:
			{
				QByteArray basePath = rawValue(ServerData, "REQUEST_URI");
				basePath.chop(queryStringLength);
				url.setEncodedPath(basePath);
				if(part == RequestUrl)
				{
					url.setEncodedQuery(rawValue(ServerData, "QUERY_STRING"));
				}
				break;
			}
			default:
				qFatal("Unknown URL part: %d", part);
		}
		return url;
	}

	QList<QNetworkCookie> Request::cookies() const
	{
		QList<QNetworkCookie> cookies;
		for(ClientIODevice::HeaderMap::ConstIterator it = d->serverData.constBegin(); it != d->serverData.constEnd(); ++it)
		{
			if(it.key().toUpper() == "HTTP_COOKIE")
			{
				QList<QByteArray> list = it.value().split(';');
				for(int i = 0; i < list.length(); ++i)
				{
					cookies.append(QNetworkCookie::parseCookies(list.at(i)));
				}
			}
		}
		return cookies;
	}

	void Request::sendCookie(const QNetworkCookie& cookie)
	{
		addHeader("set-cookie", cookie.toRawForm());
	}

	void Request::setHeader(const QByteArray& name, const QByteArray& value)
	{
		Q_ASSERT(!d->haveSentHeaders);
		d->responseHeaders[name] = value;
	}

	void Request::addHeader(const QByteArray& name, const QByteArray& value)
	{
		Q_ASSERT(!d->haveSentHeaders);
		d->responseHeaders.insertMulti(name, value);
	}

	QByteArray Request::responseHeader(const QByteArray& name)
	{
		return d->responseHeaders.value(name);
	}

	QHash<QByteArray, QByteArray> Request::rawValues(DataSource source) const
	{
		switch(source)
		{
			case GetData:
				return d->getData;
			case PostData:
				d->loadPostVariables();
				return d->postData;
			case ServerData:
				return d->serverData;
			default:
				qFatal("Unknown value type: %d", source);
		}
		return QHash<QByteArray, QByteArray>();
	}

	QByteArray Request::rawValue(DataSource source, const QByteArray& name) const
	{
		return rawValues(source).value(name);
	}

	QString Request::value(DataSource source, const QByteArray& name) const
	{
		return QUrl::fromPercentEncoding(rawValue(source, name));
	}

	Request::~Request()
	{
		emit finished(this);
		delete d;
	}
}
