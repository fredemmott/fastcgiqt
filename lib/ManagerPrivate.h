#ifndef _FASTCGI_QT_MANAGER_PRIVATE_H
#define _FASTCGI_QT_MANAGER_PRIVATE_H

#include "RecordHeader.h"
#include "Responder.h"
#include "Request.h"

#include <QHash>
#include <QObject>
#include <QStringList>

class QLocalSocket;
class QSignalMapper;

namespace FastCgiQt
{
	class ManagerPrivate : public QObject
	{
		Q_OBJECT
		public:
			ManagerPrivate(ResponderGenerator responderGenerator, QObject* parent = NULL);
		private slots:
			void processSocketData(int socket);
			void listen();
		private:
			bool processNewRecord(int socket);
			bool processRecordData(int socket);
			void beginRequest(const RecordHeader& header, const QByteArray& data);
			void loadParameters(const RecordHeader& header, const QByteArray& data);
			void readStandardInput(const RecordHeader& header, const QByteArray& data);
			void respond(quint16 requestId);

			void lockSocket(int socket);
			void releaseSocket(int socket);

			ResponderGenerator m_responderGenerator;
			QLocalSocket* m_socket;

			/// Used to map readyRead signals to processSocketData(int socket)
			QSignalMapper* m_socketMapper;
			QStringList m_allowedAddresses;

			// FastCGI spec says request IDs will be tightly backed near zero.
			QVector<Request> m_requests;
			QVector<bool> m_closeSocketOnExit;
			QHash<int, RecordHeader> m_socketHeaders;
	};
};

#endif
