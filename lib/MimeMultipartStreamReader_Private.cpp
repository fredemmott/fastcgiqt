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
#include "MimeMultipartStreamReader_Private.h"

#include <QDebug>
#include <QStringList>

namespace FastCgiQt
{
	MimeMultipartStreamReader::Private::Private(QIODevice* device)
	: m_device(device)
	, m_tokenType(NoToken)
	, m_content(0)
	{
	}

	MimeMultipartStreamReader::Private::Private(QIODevice* device, const QByteArray& boundary)
	: m_device(device)
	, m_boundary(boundary)
	, m_tokenType(NoToken)
	, m_content(0)
	{
	}

	MimeMultipartStreamReader::Private::~Private()
	{
		delete m_content;
	}

	void MimeMultipartStreamReader::Private::parseHeader()
	{
		Q_ASSERT(m_tokenType == DocumentHeader);
		if(m_headerName.toUpper() == "CONTENT-TYPE")
		{
			const QStringList parts = m_headerValue.split("; ");
			Q_FOREACH(const QString& part, parts)
			{
				if(part.startsWith("boundary="))
				{
					const int startPosition = part.indexOf('"');
					const int endPosition = part.indexOf('"', startPosition + 1) - 1;
					const int count = endPosition - startPosition;
					Q_ASSERT(startPosition != -1);
					Q_ASSERT(endPosition != -1);
					Q_ASSERT(count > 0);
					m_boundary = part.mid(startPosition + 1, count).toLatin1();
					//qDebug() << "Parsed boundary as" << m_boundary;
				}
			}
		}
	}
};
