#pragma once

#include "OutputDevice_Backend.h"

namespace FastCgiQt
{
	class FastCgiOutputBackend : public OutputDevice::Backend
	{
		Q_OBJECT;
		public:
			/// Will be reparented to the OutputDevice
			FastCgiOutputBackend(quint16 requestId, QIODevice* socket);
			/// Wait on the socket
			bool waitForBytesWritten(int msecs);
		protected:
			/// Write a FastCGI STDOUT record
			qint64 writeData(const char* data, qint64 maxSize);
		private:
			quint16 m_requestId;
			QIODevice* m_socket;
	};
}
