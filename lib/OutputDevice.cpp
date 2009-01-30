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
#include "OutputDevice.h"

#include "StandardOutputRecord.h"

namespace FastCgiQt
{
	OutputDevice::OutputDevice(quint16 requestId, QIODevice* socket, QObject* parent)
		:
			QIODevice(parent),
			m_requestId(requestId),
			m_socket(socket)
	{
		open(QIODevice::WriteOnly);
	}

	qint64 OutputDevice::readData(char* data, qint64 maxSize)
	{
		Q_UNUSED(data);
		Q_UNUSED(maxSize);
		return -1;
	}

	qint64 OutputDevice::writeData(const char* data, qint64 maxSize)
	{
		QByteArray record = StandardOutputRecord::create(
			m_requestId,
			QByteArray::fromRawData(data, maxSize)
		);
		qint64 wrote = m_socket->write(record.constData(), record.length());
		Q_ASSERT(wrote == record.length());
		if(wrote == record.length())
		{
			return maxSize;
		}
		else
		{
			return -1;
		}
	}
}
