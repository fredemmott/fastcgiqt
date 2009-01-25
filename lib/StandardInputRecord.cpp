#include "StandardInputRecord.h"

#include "RecordHeader.h"

namespace FastCgiQt
{
	StandardInputRecord::StandardInputRecord(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::StandardInputRecord);
		m_requestId = header.requestId();
		m_streamData = QByteArray(data.constData(), header.contentLength());
	}

	quint16 StandardInputRecord::requestId() const
	{
		return m_requestId;
	}

	QByteArray StandardInputRecord::streamData() const
	{
		return m_streamData;
	}
}
