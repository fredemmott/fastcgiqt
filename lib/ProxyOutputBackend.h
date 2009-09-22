#pragma once

#include "OutputDevice_Backend.h"

namespace FastCgiQt
{
	/// Simple OutputDevice backend that just passes on stuff to another QIODevice
	class ProxyOutputBackend : public OutputDevice::Backend
	{
		Q_OBJECT;
		public:
			ProxyOutputBackend(QIODevice* source);
			bool waitForBytesWritten(int msecs);
		protected:
			qint64 writeData(const char* data, qint64 maxSize);
		private:
			QIODevice* m_source;
	};
};
