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

#include <QHash>
#include <QIODevice>

namespace FastCgiQt
{
	/** @internal Interface for accessing/sending client data.
	 *
	 * This provides access to the headers, and direct to POST data and response output as a stream (QIODevice).
	 *
	 * Response headers will be written as normal data, CGI-style. If the interface implemented by a subclass
	 * doesn't support this, conversion must be done transparently (eg, translate Status: 200 OK to HTTP/1.1 200 OK)
	 *
	 * @todo cleanup - this almost does the same thing as ClientIOInterface
	 * ClientIOInterface should probably be removed
	 * @ingroup core
	 */
	class ClientIODevice: public QIODevice
	{
		Q_OBJECT
		public:
			typedef QHash<QByteArray, QByteArray> HeaderMap;
			/// Destructor.
			virtual ~ClientIODevice();

			/** All ClientIODevices should be assumed to be sequential.
			 *
			 * @returns true
			 */
			bool isSequential() const;

			/// Return all (CGI-style) headers included in the request
			virtual HeaderMap requestHeaders() const = 0;
		protected:
			ClientIODevice(QObject* parent);
	};
}
