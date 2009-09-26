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
#include "ParametersRecord.h"

#include "RecordHeader.h"
#include "UnsignedByteArray.h"

#include <QDebug>

namespace FastCgiQt
{
	ParametersRecord::ParametersRecord(const RecordHeader& header, const QByteArray& _data)
	{
		const UnsignedByteArray data(_data);
		Q_ASSERT(header.type() == RecordHeader::ParametersRecord);
		Q_ASSERT(data.length() >= header.contentLength());

		quint16 i = 0;
		quint16 bytesToRead = header.contentLength();

		const quint8 highBitMask = 1 << 7;

		while(i < bytesToRead)
		{
			quint32 nameLength;
			quint32 valueLength;

			// See "Name-Value pairs" in the spec

			// work out name length
			if(data[i] & highBitMask)
			{
				// Four bytes of name length
				nameLength = 
					((data[i] & ~highBitMask) << 24)
					+ (data[i+1] << 16)
					+ (data[i+2] << 8)
					+ data[i+3]
				;
				i+= 4;
			}
			else
			{
				// 1 byte of name length
				nameLength = data[i++];
			}

			// ditto for value
			if(data[i] & highBitMask)
			{
				// Four bytes of value length
				valueLength = 
					((data[i] & ~highBitMask) << 24)
					+ (data[i+1] << 16)
					+ (data[i+2] << 8)
					+ data[i+3]
				;
				i+= 4;
			}
			else
			{
				// 1 byte of name length
				valueLength = data[i++];
			}

			const QByteArray name = QByteArray(&data.constData()[i], nameLength);
			i += nameLength;
			const QByteArray value = QByteArray(&data.constData()[i], valueLength);
			i += valueLength;
			m_parameters.insert(name, value);
		}
	}

	bool ParametersRecord::isEmpty() const
	{
		return m_parameters.isEmpty();
	}

	ClientIODevice::HeaderMap ParametersRecord::parameters() const
	{
		return m_parameters;
	}
}
