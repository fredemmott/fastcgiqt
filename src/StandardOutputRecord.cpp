#include "StandardOutputRecord.h"

#include "RecordHeader.h"

namespace FastCgiQt
{
	QByteArray StandardOutputRecord::create(quint16 requestId, QByteArray data)
	{
		return RecordHeader::create(requestId, RecordHeader::StandardOutputRecord, data);
	};
}
