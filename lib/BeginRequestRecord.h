#ifndef _FASTCGI_QT_BEGIN_REQUEST_RECORD_H
#define _FASTCGI_QT_BEGIN_REQUEST_RECORD_H

#include "EnumHelpers.h"
#include "fastcgi.h"

#include <QFlags>
#include <QObject>

namespace FastCgiQt
{
	class RecordHeader;
	class BeginRequestRecord
	{
		Q_GADGET
		public:
			Q_ENUMS(Role Flags);
			enum Role
			{
				ResponderRole  = FCGI_RESPONDER,
				AuthorizerRole = FCGI_AUTHORIZER,
				FilterRole     = FCGI_FILTER,

				UnknownRole,	
				FirstRole = ResponderRole,
			};
			enum Flag
			{
				KeepConnection = FCGI_KEEP_CONN,

				UnknownFlag,
				FirstFlag = KeepConnection
			};
			Q_DECLARE_FLAGS(Flags, Flag);

			BeginRequestRecord(const RecordHeader& header, const FCGI_BeginRequestBody&);

			quint16 requestId() const;
			Role role() const;
			Flags flags() const;
		private:
			quint16 m_requestId;
			Role m_role;
			Flags m_flags;
	};
};

DECLARE_DEBUG_ENUM(FastCgiQt::BeginRequestRecord, Role);
Q_DECLARE_OPERATORS_FOR_FLAGS(FastCgiQt::BeginRequestRecord::Flags);

#endif
