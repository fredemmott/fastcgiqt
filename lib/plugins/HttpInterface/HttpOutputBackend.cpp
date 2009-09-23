#include "HttpOutputBackend.h"

#include <QDebug>

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
			QByteArray headerData;
			// We need to find the "STATUS" header.
			for(int position = 0; position < blob.length(); )
			{
				// See if we're at the end of the headers
				if(blob.at(position) == '\r')
				{
					// we've hit the end
					headerData.append(blob.mid(position, -1));
					break;
				}
				// Nope, find the header name
				const int endOfName = blob.indexOf(':', position);
				Q_ASSERT(endOfName != -1);
				const int endOfValue = blob.indexOf('\r', endOfName);
				if(blob.mid(position, endOfName - position) == "STATUS")
				{
					// We've got the status header
					const QByteArray value = blob.mid(
						endOfName + 2, // ": ", not ':'
						endOfValue - endOfName
					);
					qDebug() << "Status header:" << value;
					const int endOfCode = value.indexOf(' ');
					Q_ASSERT(endOfCode != -1);
					statusCode = QString::fromLatin1(value.left(endOfCode)).toInt();
					statusString = value.mid(endOfCode + 1, -1);
					qDebug() << "Code:" << statusCode << "Text:" << statusString;
				}
				else
				{
					const QByteArray thisHeader = blob.mid(position, (endOfValue - position) + 2); // include the \r\n
					qDebug() << "Header:" << thisHeader;
					headerData.append(thisHeader);
				}
				position = endOfValue + 2;
			}
			Q_ASSERT(statusCode != -1);
			Q_ASSERT(!statusString.isEmpty());
			qDebug() << "Code:" << statusCode << "String:" << statusString;
			qDebug() << "Other headers:" << headerData;
			::evhttp_send_reply_start(m_request, statusCode, statusString);
			m_state = SentHeaders;
			const qint64 sent = writeData(headerData, headerData.length());
			if(sent == headerData.length())
			{
				return maxSize;
			}
			else
			{
				return -1;
			}
		}
		struct evbuffer* buffer = ::evbuffer_new();
		::evbuffer_add(buffer, data, maxSize);
		::evhttp_send_reply_chunk(m_request, buffer);
		::evbuffer_free(buffer);
		return maxSize;
	}
};
