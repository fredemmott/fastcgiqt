#pragma once

#include "OutputDevice.h"

namespace FastCgiQt
{
	class OutputDevice::Backend : public QIODevice
	{
		Q_OBJECT;
		protected:
			/// Returns -1
			qint64 readData(char* data, qint64 maxSize);

			/// Reparented by OutputDevice
			Backend();
	};
};
