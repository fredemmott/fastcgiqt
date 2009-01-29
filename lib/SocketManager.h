#ifndef _FASTCGI_QT_SOCKET_MANAGER_H
#define _FASTCGI_QT_SOCKET_MANAGER_H

#include "RecordHeader.h"
#include "Responder.h"
#include "Request.h"

class QLocalSocket;

namespace FastCgiQt
{
	class InputDevice;
	class SocketManager : public QObject
	{
		Q_OBJECT
		public:
			SocketManager(ResponderGenerator responderGenerator, int socketId, QObject* parent = NULL);
			~SocketManager();
		private:
			void queueSocketCheck();

			bool processNewRecord();
			bool processRecordData();
			void beginRequest(const QByteArray& data);
			bool loadParameters(const QByteArray& data);
			void readStandardInput(const QByteArray& data);
			void respond();
		private slots:
			void processSocketData();
		private:
			RecordHeader m_recordHeader;
			ResponderGenerator m_responderGenerator;
			QLocalSocket* m_socket;

			// FastCGI spec says request IDs will be tightly packed near zero.
			QVector<Request> m_requests;
			QVector<bool> m_closeSocketOnExit;
			QVector<InputDevice* > m_inputDevices;
	};
}
#endif
