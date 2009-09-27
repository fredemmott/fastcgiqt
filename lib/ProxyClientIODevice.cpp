#include "ProxyClientIODevice.h"

namespace FastCgiQt
{
	ProxyClientIODevice::ProxyClientIODevice(const HeaderMap& headers, QIODevice* source, QObject* parent)
	: ClientIODevice(parent)
	, m_headers(headers)
	, m_source(source)
	{
		open(QIODevice::ReadWrite | QIODevice::Unbuffered);
	}

	qint64 ProxyClientIODevice::readData(char* data, qint64 maxSize)
	{
		return m_source->read(data, maxSize);
	}

	ClientIODevice::HeaderMap ProxyClientIODevice::requestHeaders() const
	{
		return m_headers;
	}

	bool ProxyClientIODevice::waitForBytesWritten(int msec)
	{
		return m_source->waitForBytesWritten(msec);
	}

	qint64 ProxyClientIODevice::writeData(const char* data, qint64 maxSize)
	{
		return m_source->write(data, maxSize);
	}
};
