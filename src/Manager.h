#ifndef _FASTCGI_QT_MANAGER_H
#define _FASTCGI_QT_MANAGER_H

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
	class Manager : public QObject
	{
		Q_OBJECT
		public:
			Manager(ResponderGenerator responderGenerator, QObject* parent = NULL);
		private slots:
			void processSocketData(int socket);
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
			QHash<int, RecordHeader> m_socketHeaders;
	};
};

#endif
