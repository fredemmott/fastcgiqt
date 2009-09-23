#include "HttpOutputBackend.h"

#include <QDebug>
#include <QPair>

namespace FastCgiQt
{
	HttpOutputBackend::HttpOutputBackend(struct evhttp_request* request)
	: m_request(request)
	, m_state(NewReply)
	{
		open(QIODevice::WriteOnly | QIODevice::Unbuffered);
	}

	HttpOutputBackend::~HttpOutputBackend()
	{
		qDebug() << "Sending reply end";
		::evhttp_send_reply_end(m_request);
	}

	qint64 HttpOutputBackend::writeData(const char* data, qint64 maxSize)
	{
		if(m_state == NewReply)
		{
			//FIXME: assumes (currently true) that OutputDevice will send all the header data in one go
			QByteArray blob(data, maxSize);
			int statusCode = -1;
			QByteArray statusString;
			// We need to find the "STATUS" header.
			for(int position = 0; position < blob.length(); )
			{
				// See if we're at the end of the headers
				if(blob.at(position) == '\r')
				{
					// we've hit the end
					break;
				}
				// Nope, find the header name
				const int endOfName = blob.indexOf(':', position);
				Q_ASSERT(endOfName != -1);
				const int endOfValue = blob.indexOf('\r', endOfName);
				const QByteArray name = blob.mid(position, endOfName - position);
				const QByteArray value = blob.mid(
					endOfName + 2, // ": ", not ':'
					endOfValue - (endOfName + 2)
				);
				if(name == "STATUS")
				{
					// We've got the status header
					const int endOfCode = value.indexOf(' ');
					Q_ASSERT(endOfCode != -1);
					statusCode = QString::fromLatin1(value.left(endOfCode)).toInt();
					statusString = value.mid(endOfCode + 1, -1);
				}
				else
				{
					::evhttp_add_header(m_request->output_headers, name, value);
				}
				position = endOfValue + 2;
			}
			Q_ASSERT(statusCode != -1);
			Q_ASSERT(!statusString.isEmpty());
			::evhttp_send_reply_start(m_request, statusCode, statusString);
			m_state = SentHeaders;
			return maxSize;
		}
		struct evbuffer* buffer = ::evbuffer_new();
		::evbuffer_add(buffer, data, maxSize);
		::evhttp_send_reply_chunk(m_request, buffer);
		::evbuffer_free(buffer);
		return maxSize;
	}
};
