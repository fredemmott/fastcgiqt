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
#include "Request_Private.h"

#include <QCoreApplication>
#include <QDebug>
#include <QHash>
#include <QPair>
#include <QTime>
#include <QUrl>

namespace FastCgiQt
{
	Request::Private::Private(ClientIODevice* d)
	: device(d)
	, haveSentHeaders(false)
	, postDataMode(UnknownPostData)
	, serverData(d->requestHeaders())
	{
		d->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
		getData = parseQueryString(serverData.value("QUERY_STRING"));
		// Default header values:
		responseHeaders.insert("STATUS", "200 OK");
		responseHeaders.insert("CONTENT-TYPE", "text/html; charset=UTF-8");
	}

	Request::Private::~Private()
	{
		delete device;
	}

	ClientIODevice::HeaderMap Request::Private::parseQueryString(const QByteArray& queryString)
	{
		ClientIODevice::HeaderMap out;

		QUrl queryStringParser;
		queryStringParser.setEncodedQuery(queryString);

		typedef QPair<QByteArray, QByteArray> HeaderPair;
		Q_FOREACH(HeaderPair header, queryStringParser.encodedQueryItems())
		{
			out.insert(header.first, header.second);
		}
		return out;
	}

	void Request::Private::loadPostVariables()
	{
		qDebug() << Q_FUNC_INFO;
		switch(postDataMode)
		{
			case RawPostData:
				qFatal("Tried to read post variables after reading raw post data.");
				break;
			case ValuesPostData:
				// already loaded
				break;
			case UnknownPostData:
				postDataMode = ValuesPostData;
				if(serverData.values("CONTENT_TYPE").startsWith("application/x-www-form-urlencoded"))
				{
					///@todo pay attention if content-type includes "; charset=FOO" - at the moment, assume UTF8
					QByteArray data = device->readAll();
					const int expected = QString::fromLatin1(serverData.value("CONTENT_LENGTH")).toInt();
					if(data.length() < expected)
					{
						QTime timer;
						timer.start();
						Q_FOREVER
						{
							QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 5000); // even if nothing new comes, on any connection, for 5 seconds, give poll
							data.append(device->readAll());
							if(data.length() >= expected)
							{
								break;
							}
							else if(timer.elapsed() > 1000 * 60 * 5) ///@fixme make the 5-minute POST data timeout configurable
							{
								qWarning("Timeout exceeded on post data upload, aborting.");
								return;
							}
						}
					}
					Q_ASSERT(data.length() == expected); // not >
					postData = parseQueryString(data);
				}
				break;
		}
	}
};
