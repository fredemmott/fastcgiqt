#include "RecordHeader.h"

#include <QDebug>
#include "EnumHelpers.h"

namespace FastCgiQt
{
	RecordHeader::RecordHeader(const FCGI_Header& record)
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
}
QDebug operator<<(QDebug dbg, FastCgiQt::RecordHeader::RecordType type)
{
	dbg.nospace() << static_cast<int>(type) << " (" << NAME_FROM_ENUM(FastCgiQt::RecordHeader,RecordType,type) << ")";
	return dbg.space();
}
