/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#pragma once

#include "CommunicationInterface_Worker.h"
#include "RecordHeader.h"
#include "Responder.h"
#include "Request.h"

#include <QHash>

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
	class FastCgiSocketManager : public CommunicationInterface::Worker
	{
		Q_OBJECT
		public:
			/** Create a FastCgiSocketManager.
			 * @param responderGenerator is a function to use to
			 * 	create Responder objects.
			 * @param socketId is the file descriptor for a socket.
			 * @param parent is a parent object.
			 */
			FastCgiSocketManager(Responder::Generator responderGenerator, int socketId, QObject* parent = NULL);
			/// Destroy this socket manager.
			~FastCgiSocketManager();
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
			void cleanupResponder(Responder* responder);
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
			/// The socket this FastCgiSocketManager managers.
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

			/// Responder -> requestId map
			QHash<Responder*, quint64> m_requestMap;
	};
}
