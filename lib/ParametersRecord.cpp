#include "ParametersRecord.h"

#include "RecordHeader.h"

#include <QDebug>

namespace FastCgiQt
{
	ParametersRecord::ParametersRecord(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::ParametersRecord);
		Q_ASSERT(data.length() >= header.contentLength());
		m_requestId = header.requestId();

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
					((data[i] ^ highBitMask) << 24)
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
					((data[i] ^ highBitMask) << 24)
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

			QString name = QString::fromUtf8(&data.constData()[i], nameLength);
			i += nameLength;
			QString value = QString::fromUtf8(&data.constData()[i], valueLength);
			i += valueLength;
			m_parameters.insert(name, value);
		}
	}

	bool ParametersRecord::isEmpty() const
	{
		return m_parameters.isEmpty();
	}

	quint16 ParametersRecord::requestId() const
	{
		return m_requestId;
	}

	QHash<QString, QString> ParametersRecord::parameters() const
	{
		return m_parameters;
	}
}
