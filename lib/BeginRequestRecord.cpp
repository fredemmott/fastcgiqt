#include "BeginRequestRecord.h"

#include "RecordHeader.h"

namespace FastCgiQt
{
	BeginRequestRecord::BeginRequestRecord(const RecordHeader& header, const FCGI_BeginRequestBody& body)
	{
		Q_ASSERT(header.type() == RecordHeader::BeginRequestRecord);
		int role = (body.roleB1 << 8) | body.roleB0;
		if(role < FirstRole || role >= UnknownRole)
		{
			qFatal("Unknown role '%d'", role);
		}
		m_role = static_cast<Role>(role);
		m_flags = static_cast<Flag>(body.flags);
	}

	BeginRequestRecord::Role BeginRequestRecord::role() const
	{
		return m_role;
	}

	BeginRequestRecord::Flags BeginRequestRecord::flags() const
	{
		return m_flags;
	}
}

DEFINE_DEBUG_ENUM(FastCgiQt::BeginRequestRecord, Role);
