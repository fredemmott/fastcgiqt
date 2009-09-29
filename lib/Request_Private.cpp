#include "Request_Private.h"

#include <QHash>
#include <QPair>
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
		responseHeaders.insert("STATUS", "200 OK"); // just in case
		getData = parseQueryString(serverData.value("QUERY_STRING"));
	}

	Request::Private::~Private()
	{
		delete device;
	}

	ClientIODevice::HeaderMap Request::Private::parseQueryString(const QByteArray& queryString)
	{
		ClientIODevice::HeaderMap out;

		QUrl queryStringParser;
		queryStringParser.setEncodedQuery(serverData.value("QUERY_STRING"));

		typedef QPair<QByteArray, QByteArray> HeaderPair;
		Q_FOREACH(HeaderPair header, queryStringParser.encodedQueryItems())
		{
			out.insert(header.first, header.second);
		}
		return out;
	}

	void Request::Private::loadPostVariables()
	{
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
					const QByteArray data = device->readAll();
					Q_ASSERT(data.length() == QString::fromLatin1(serverData.value("CONTENT_LENGTH")).toInt());
					postData = parseQueryString(data);
				}
				break;
		}
	}
};
