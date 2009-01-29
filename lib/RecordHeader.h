#ifndef _FASTCGI_QT_RECORD_HEADER_H
#define _FASTCGI_QT_RECORD_HEADER_H

#include "EnumHelpers.h"

#include "fastcgi.h"

#include <QByteArray>
#include <QObject>

namespace FastCgiQt
{
	/** @internal
	 * @brief Class to parse a FastCGI record header.
	 *
	 * This will parse a FastCGI record-header struct, and provide access to
	 * the information in it through more convenient types.
	 */
	class RecordHeader
	{
		Q_GADGET
		public:
			/// The type of record.
			enum RecordType
			{
				BeginRequestRecord    = FCGI_BEGIN_REQUEST,     ///< The first record of a request
				AbortRequestRecord    = FCGI_ABORT_REQUEST,     ///< An instruction to abort processing of a request
				EndRequestRecord      = FCGI_END_REQUEST,       ///< Marker that a request has finished being processed.
				ParametersRecord      = FCGI_PARAMS,            ///< Record containing parameters from the server.
				StandardInputRecord   = FCGI_STDIN,             ///< Record containing POST data.
				StandardOutputRecord  = FCGI_STDOUT,            ///< Record containing data to send to the client.
				StandardErrorRecord   = FCGI_STDERR,            ///< Record containing error data.
				DataRecord            = FCGI_DATA,              ///< Miscellaneous data.
				GetValuesRecord       = FCGI_GET_VALUES,        ///< Request for application configuration data.
				GetValuesResultRecord = FCGI_GET_VALUES_RESULT, ///< Application configuration data.
				UnknownRecordType     = FCGI_UNKNOWN_TYPE,      ///< Unknown record type.

				FirstRecordType       = BeginRequestRecord      ///< Marker for the lowest value of RecordType
			};
			Q_ENUMS(RecordType);

			/// Construct an invalid RecordHeader.
			RecordHeader();
			/// Construct a valid record header, and parse an FCGI header struct.
			RecordHeader(const FCGI_Header& record);

			/** Whether or not this RecordHeader is valid.
			 *
			 * @see RecordHeader()
			 * @see RecordHeader(const FCGI_Header& record)
			 */
			bool isValid() const;
			/// The type of this record.
			RecordType type() const;
			/// The FastCGI request ID.
			quint16 requestId() const;
			/** The length of the record data.
			 *
			 * @see payloadLength() const
			 */
			quint16 contentLength() const;
			/** The length of the data payload.
			 *
			 * This is the length of the content, plus some padding.
			 *
			 * @see contentLength() const
			 */
			quint16 payloadLength() const;
		
			/** Create a record.
			 *
			 * This for sending records to the web server.
			 *
			 * @param requestId is the FastCGI request ID related to
			 * 	this record.
			 * @param type is the type of record to create.
			 * @param data is the data to include in the record.
			 *
			 * @returns the record, in a QByteArray.
			 */
			static QByteArray create(quint16 requestId, RecordType type, const QByteArray& data);
		private:
			RecordType m_type;
			quint16 m_requestId;
			quint16 m_contentLength;
			quint16 m_payloadLength;
			bool m_isValid;
	};
}

DECLARE_DEBUG_ENUM(FastCgiQt::RecordHeader, RecordType);

#endif
