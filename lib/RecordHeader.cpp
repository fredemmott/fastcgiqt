#include "RecordHeader.h"

#include <QDebug>

namespace FastCgiQt
{
	RecordHeader::RecordHeader()
		:
			m_isValid(false)
	{
	}

	RecordHeader::RecordHeader(const FCGI_Header& record)
		:
			m_isValid(true)
	{
		if(record.version != 1)
		{
			qFatal("Unknown FastCGI version: %d", static_cast<int>(record.version));
		}
		if(record.type >= UnknownRecordType || record.type < FirstRecordType)
		{
			qFatal("Unknown record type: %d", static_cast<int>(record.type));
		}
		m_type = static_cast<RecordType>(record.type);
		m_requestId = (record.requestIdB1 << 8) | record.requestIdB0;
		m_contentLength = (record.contentLengthB1 << 8) | record.contentLengthB0;
		m_payloadLength = m_contentLength + record.paddingLength;
	}

	bool RecordHeader::isValid() const
	{
		return m_isValid;
	}

	RecordHeader::RecordType RecordHeader::type() const
	{
		return m_type;
	}

	quint16 RecordHeader::requestId() const
	{
		return m_requestId;
	}

	quint16 RecordHeader::contentLength() const
	{
		return m_contentLength;
	}

	quint16 RecordHeader::payloadLength() const
	{
		return m_payloadLength;
	}
	
	QByteArray RecordHeader::create(quint16 requestId, RecordType type, const QByteArray& data)
	{
		FCGI_Header header;
		::memset(&header, 0, FCGI_HEADER_LEN);

		header.version = 1;
		header.type = type;
		///@todo deal with data.length >= 2^16
		Q_ASSERT(data.length() < 2^16);
		header.requestIdB1 = requestId >> 8;
		header.requestIdB0 = requestId & 0xff;
		header.contentLengthB1 = data.length() >> 8;
		header.contentLengthB0 = data.length() & 0xff;

		QByteArray returnData(QByteArray::fromRawData(reinterpret_cast<const char*>(&header), FCGI_HEADER_LEN));
		returnData.append(data);
		return returnData;
	}
}

DEFINE_DEBUG_ENUM(FastCgiQt::RecordHeader, RecordType);
