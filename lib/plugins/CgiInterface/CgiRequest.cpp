/* LICENSE NOTICE
	Copyright (c) 2010, Frederick Emmott <mail@fredemmott.co.uk>

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
#include "CgiRequest.h"

#include <QDebug>

#ifdef Q_OS_WIN32
	#define environ _environ
#endif
#include <stdio.h>

extern char** environ;

namespace FastCgiQt
{
	CgiRequest::CgiRequest(QObject* parent)
	: ClientIODevice(parent)
	{
		open(QIODevice::ReadWrite | QIODevice::Unbuffered);
		// Read headers
		for(int i = 0; environ[i]; ++i)
		{
			QList<QByteArray> parts = QByteArray(environ[i]).split('=');
			if(parts.count() > 1)
			{
				const QByteArray key = parts.takeFirst();
				QByteArray value;
				Q_FOREVER
				{
					value.append(parts.takeFirst());
					if(!parts.isEmpty())
					{
						value.append('=');
					}
					else
					{
						break;
					}
				}
				m_requestHeaders.insertMulti(key, value);
			}
		}
	}

	CgiRequest::~CgiRequest()
	{
		fflush(stdout);
	}

	ClientIODevice::HeaderMap CgiRequest::requestHeaders() const
	{
		return m_requestHeaders;
	}

	qint64 CgiRequest::readData(char* data, qint64 maxSize)
	{
		return fread(data, 1, maxSize, stdin);
	}

	qint64 CgiRequest::writeData(const char* data, qint64 maxSize)
	{
		return fwrite(data, 1, maxSize, stdout);
	}
}
