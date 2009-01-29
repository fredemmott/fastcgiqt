#ifndef _FASTCGI_QT_PARAMETERS_RECORD_H
#define _FASTCGI_QT_PARAMETERS_RECORD_H

#include <QByteArray>
#include <QHash>
#include <QString>

namespace FastCgiQt
{
	class RecordHeader;
	/** @internal
	 * @brief Class for reading FastCGI parameter records.
	 *
	 * These contain the data that is presented in environment variables in
	 * CGI.
	 */
	class ParametersRecord
	{
		public:
			/** Create a ParametersRecord object.
			 *
			 * @param header is the header for the current record.
			 * @param data is the payload for this record.
			 */
			ParametersRecord(const RecordHeader& header, const QByteArray& data);
			/// Whether or not an empty record was given.
			bool isEmpty() const;
			/** The parameters given in this record.
			 *
			 * @see isEmpty()
			 */
			QHash<QString, QString> parameters() const;
		private:
			QHash<QString, QString> m_parameters;
	};
}

#endif
