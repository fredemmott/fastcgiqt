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
