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
#ifndef _FASTCGI_QT_STANDARD_OUTPUT_RECORD_H
#define _FASTCGI_QT_STANDARD_OUTPUT_RECORD_H

#include <QByteArray>

namespace FastCgiQt
{
	/** @internal
	 * @brief Class providing function to create a FastCGI STDOUT record.
	 *
	 * These records contain data to be sent to the client.
	 */
	class StandardOutputRecord
	{
		public:
			/** Create a FastCGI STDOUT record.
			 *
			 * @param requestId is the FastCGI ID for the request
			 * 	that this data is for.
			 * @param data is the data to send to the client.
			 *
			 * @returns a FastCGI STDOUT record in a QByteArray.
			 */
			static QByteArray create(quint16 requestId, QByteArray data);
	};
};

#endif
