#ifndef _FASTCGI_QT_RECORD_HEADER_H
#define _FASTCGI_QT_RECORD_HEADER_H

#include "EnumHelpers.h"

#include "fastcgi.h"

#include <QByteArray>
#include <QObject>

namespace FastCgiQt
{
	class RecordHeader
	{
		Q_GADGET
		public:
			enum RecordType
			{
				BeginRequestRecord    = FCGI_BEGIN_REQUEST,
				AbortRequestRecord    = FCGI_ABORT_REQUEST,
				EndRequestRecord      = FCGI_END_REQUEST,
				ParametersRecord      = FCGI_PARAMS,
				StandardInputRecord   = FCGI_STDIN,
				StandardOutputRecord  = FCGI_STDOUT,
				StandardErrorRecord   = FCGI_STDERR,
				DataRecord            = FCGI_DATA,
				GetValuesRecord       = FCGI_GET_VALUES,
				GetValuesResultRecord = FCGI_GET_VALUES_RESULT,
				UnknownRecordType     = FCGI_UNKNOWN_TYPE,

				FirstRecordType       = BeginRequestRecord
			};
			Q_ENUMS(RecordType);

			RecordHeader();
			RecordHeader(const FCGI_Header& record);

			bool isValid() const;
			RecordType type() const;
			quint16 requestId() const;
			quint16 contentLength() const;
			quint16 payloadLength() const;
		
			/// Actually creates the entire record
			static QByteArray create(quint16 requestId, RecordType type, const QByteArray& data);
		private:
			RecordType m_type;
			quint16 m_requestId;
			quint16 m_contentLength;
			quint16 m_payloadLength;
			bool m_isValid;
	};
}

DECLARE_DEBUG_ENUM(FastCgiQt::RecordHeader, RecordType);

#endif
