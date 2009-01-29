#include "StandardInputRecord.h"

#include "RecordHeader.h"

namespace FastCgiQt
{
	StandardInputRecord::StandardInputRecord(const RecordHeader& header, const QByteArray& data)
	{
		Q_ASSERT(header.type() == RecordHeader::StandardInputRecord);
		m_streamData = QByteArray(data.constData(), header.contentLength());
	}

	QByteArray StandardInputRecord::streamData() const
	{
		return m_streamData;
	}
}
