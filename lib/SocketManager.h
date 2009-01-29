#ifndef _FASTCGI_QT_SOCKET_MANAGER_H
#define _FASTCGI_QT_SOCKET_MANAGER_H

#include "RecordHeader.h"
#include "Responder.h"
#include "Request.h"

class QLocalSocket;

namespace FastCgiQt
{
	class InputDevice;
	/** @internal
	 * @brief Class managing all FastCGI requests on a specified socket.
	 *
	 * If requested by the web server, this will delete itself, and close
	 * the socket.
	 *
	 * There can be multiple simultaneous requests over one socket.
	 */
	class SocketManager : public QObject
	{
		Q_OBJECT
		public:
			/** Create a SocketManager.
			 * @param responderGenerator is a function to use to
			 * 	create Responder objects.
			 * @param socketId is the file descriptor for a socket.
			 * @param parent is a parent object.
			 */
			SocketManager(Responder::Generator responderGenerator, int socketId, QObject* parent = NULL);
			/// Destroy this socket manager.
			~SocketManager();
		private:
			/// Put a call to processSocketData() into the event queue.
			void queueSocketCheck();

			/// Process the header of a FastCGI record.
			bool processNewRecord();
			/** Process the data of a FastCGI record.
			 *
			 * This will call one out of beginRequest,
			 * loadParameters(), or readStandardInput(), depending
			 * on the record type given in the header.
			 *
			 * Once an empty parameters record has been received, it
			 * will call respond().
			 */
			bool processRecordData();
			/// Start a new request.
			void beginRequest(const QByteArray& data);
			/// Process a parameters record.
			bool loadParameters(const QByteArray& data);
			/// Process a STDIN record.
			void readStandardInput(const QByteArray& data);
			/// Create a Responder, run it, and cleanup.
			void respond();
		private slots:
			/** Process new data on the socket.
			 *
			 * If there is sufficient data, this will call either
			 * processNewRecord() or processRecordData(), depending
			 * on the current state.
			 */
			void processSocketData();
		private:
			/// The header of the current record.
			RecordHeader m_recordHeader;
			/// The function to use to create new Responder objects.
			Responder::Generator m_responderGenerator;
			/// The socket this SocketManager managers.
			QLocalSocket* m_socket;

			// FastCGI spec says request IDs will be tightly packed near zero.

			/** The requests currently being processed.
			 *
			 * This is indexed by request id.
			 */
			QVector<Request> m_requests;
			/** If the socket is supposed to be closed.
			 *
			 * This is indexed by request id.
			 *
			 * If a value is true, once the request ID in
			 * question has been processed, deleteLater() will be
			 * called on this object.
			 */
			QVector<bool> m_closeSocketOnExit;

			/** InputDevice objects for the various requests.
			 *
			 * This is indexed by request id.
			 */
			QVector<InputDevice* > m_inputDevices;
	};
}
#endif
