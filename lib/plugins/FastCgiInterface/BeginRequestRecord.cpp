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
