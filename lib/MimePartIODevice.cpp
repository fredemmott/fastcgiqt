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
#include "MimePartIODevice.h"
#include "MimePartIODevice_Private.h"

#include <QDebug>

#include <limits>

namespace FastCgiQt
{
	MimePartIODevice::MimePartIODevice(QIODevice* source, const QByteArray& boundary, QObject* parent)
	: QIODevice(parent)
	, d(new Private(source, boundary))
	{
		Q_ASSERT(!boundary.isEmpty());
		connect(source, SIGNAL(readyRead()), SIGNAL(readyRead()));
	}

	MimePartIODevice::~MimePartIODevice()
	{
		delete d;
	}

	bool MimePartIODevice::isSequential() const
	{
		return true;
	}

	QByteArray MimePartIODevice::boundary() const
	{
		return d->m_boundary;
	}

	QIODevice* MimePartIODevice::source() const
	{
		return d->m_source;
	}

	qint64 MimePartIODevice::readData(char* data, qint64 maxSize)
	{
		if(d->m_atEnd)
		{
			return -1;
		}
		Q_ASSERT(maxSize < std::numeric_limits<int>().max());
		const QByteArray peekedData = d->m_source->peek(maxSize);
		const int fullBoundaryIndex = d->m_matcher.indexIn(peekedData);
		if(fullBoundaryIndex != -1)
		{
			const qint64 bytesRead = d->m_source->read(data, fullBoundaryIndex);
			Q_ASSERT(bytesRead == fullBoundaryIndex);
			if(bytesRead == fullBoundaryIndex)
			{
				d->m_source->getChar(0);
				d->m_atEnd = true;
			}
			return bytesRead;
		}

		// Might have the start of a boundary... be careful
		const QByteArray pattern = d->m_matcher.pattern();
		int chop = 0;
		for(int i = 1; i < pattern.length(); ++i)
		{
			if(pattern.left(i) == peekedData.right(i))
			{
				chop = i;
				break;
			}
		}
		const qint64 toRead = peekedData.length() - chop;
		const qint64 bytesRead = d->m_source->read(data, toRead);
		Q_ASSERT(bytesRead == toRead);
		return bytesRead;
	}

	qint64 MimePartIODevice::writeData(const char* data, qint64 maxSize)
	{
		Q_UNUSED(data);
		Q_UNUSED(maxSize);
		return -1;
	}
};
