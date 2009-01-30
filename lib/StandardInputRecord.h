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
#ifndef _FASTCGI_QT_STANDARD_INPUT_RECORD_H
#define _FASTCGI_QT_STANDARD_INPUT_RECORD_H

#include <QByteArray>

namespace FastCgiQt
{
	class RecordHeader;
	/** @internal
	 * @brief Class to parse a FastCGI standard input record.
	 *
	 * These records contain POST data.
	 *
	 * All this class does is chop off the padding.
	 */
	class StandardInputRecord
	{
		public:
			/** Create a StandardInputRecord.
			 *
			 * @param header is the header for this record.
			 * @param data is the data of this record.
			 */
			StandardInputRecord(const RecordHeader& header, const QByteArray& data);
			/// The actual POST data in this record.
			QByteArray streamData() const;
		private:
			QByteArray m_streamData;
	};
}

#endif
