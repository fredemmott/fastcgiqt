#include "OutputDevice_Backend.h"

namespace FastCgiQt
{
	OutputDevice::Backend::Backend()
	: QIODevice()
	{
	}

	qint64 OutputDevice::Backend::readData(char* data, qint64 maxSize)
	{
		Q_UNUSED(data);
		Q_UNUSED(maxSize);
		return -1;
	}
};
