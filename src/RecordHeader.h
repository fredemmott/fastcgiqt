#ifndef _FASTCGI_QT_RECORD_HEADER_H
#define _FASTCGI_QT_RECORD_HEADER_H

#include "fastcgi.h"

#include <QByteArray>
#include <QObject>

namespace FastCgiQt
{
	class RecordHeader : public QObject
	{
		Q_OBJECT
		public:
			enum RecordType
			{
				BeginRequestRecord = 1,
				AbortRequestRecord = 2,
				EndRequestRecord = 3,
				ParamsRecord = 4,
				StdInRecord = 5,
				StdOutRecord = 6,
				StdErrRecord = 7,
				DataRecord = 8,
				GetValuesRecord = 9,
				GetValuesResultRecord = 10,
				UnknownRecordType = 11,
				FirstRecordType = BeginRequestRecord
			};
			Q_ENUMS(RecordType);

			RecordHeader(const FCGI_Header& record);

			RecordType type() const;
			quint16 requestId() const;
			quint16 contentLength() const;
		private:
			RecordType m_type;
			quint16 m_requestId;
			quint16 m_contentLength;
	};
}

QDebug operator<<(QDebug dbg, FastCgiQt::RecordHeader::RecordType type);

#endif
