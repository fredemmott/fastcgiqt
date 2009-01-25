#ifndef _FASTCGI_QT_STANDARD_INPUT_RECORD_H
#define _FASTCGI_QT_STANDARD_INPUT_RECORD_H

#include <QByteArray>

namespace FastCgiQt
{
	class RecordHeader;
	class StandardInputRecord
	{
		public:
			StandardInputRecord(const RecordHeader& header, const QByteArray& data);
			quint16 requestId() const;
			QByteArray streamData() const;
		private:
			quint16 m_requestId;
			QByteArray m_streamData;
	};
}

#endif
