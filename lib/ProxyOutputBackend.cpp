#include "ProxyOutputBackend.h"

namespace FastCgiQt
{
	ProxyOutputBackend::ProxyOutputBackend(QIODevice* source)
	: OutputDevice::Backend()
	, m_source(source)
	{
		open(QIODevice::WriteOnly | QIODevice::Unbuffered);
	}

	bool ProxyOutputBackend::waitForBytesWritten(int msecs)
	{
		return m_source->waitForBytesWritten(msecs);
	}

	qint64 ProxyOutputBackend::writeData(const char* data, qint64 maxSize)
	{
		return m_source->write(data, maxSize);
	}
};
