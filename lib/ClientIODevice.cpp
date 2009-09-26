#include "ClientIODevice.h"

namespace FastCgiQt
{
	ClientIODevice::ClientIODevice()
	: QIODevice()
	{
	}

	ClientIODevice::~ClientIODevice()
	{
	}

	bool ClientIODevice::isSequential() const
	{
		return true;
	}

	QByteArray ClientIODevice::requestHeader(const QByteArray& name) const
	{
		return requestHeaders().value(name);
	}
};
