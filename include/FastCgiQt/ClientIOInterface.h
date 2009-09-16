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
#ifndef _FASTCGI_QT_CLIENT_IO_INTERFACE_H
#define _FASTCGI_QT_CLIENT_IO_INTERFACE_H

#include "Request.h"

#include <QObject>
#include <QTextStream>

class QIODevice;
class QNetworkCookie;

namespace FastCgiQt
{
	/** Interface for accessing/sending client data.
	 * @ingroup core
	 */
	class ClientIOInterface : public QObject
	{
		Q_OBJECT
		public:
			/// Destructor.
			~ClientIOInterface();
		protected:
			/** Construct a ClientIOInterface
			 *
			 * You shouldn't actually have to do anything with these
			 * parameters except pass them through to this
			 * constructor; especially, do not use the socket
			 * directly.
			 *
			 * @param request contains information on the request.
			 * @param socket is the socket being used for FastCGI in
			 * 	this request.
			 * @param inputDevice is a streaming input device.
			 * @param parent is a QObject* parent.
			 */
			ClientIOInterface(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent = NULL);

			/// Class containing information about the request.
			const Request& request;
			/** Input text stream (POST data).
			 *
			 * This is the HTTP post data, as a text stream.
			 * If you need the raw device, use in.device().
			 *
			 * The Request class has other (possible more
			 * convenient) ways to access this data, although
			 * not as a stream.
			 *
			 * @see Request::postData() const
			 * @see Request::postData(const QString&) const
			 * @see Request::content()
			 * @see Request::contentType()
			 * @see Request::contentLength()
			 * @see Request::haveAllContent()
			 * @see Request::waitForAllContent()
			 */
			QTextStream in;

			/** Output text stream.
			 *
			 * Text written to this stream will be sent to the
			 * client.
			 *
			 * If you need the raw device, use out.device().
			 */
			QTextStream out;

			/** Return a header sent/due to be sent to the client.
			 *
			 * @returns a null QString if header @p name is not set.
			 * @returns the value of header @p name otherwise.
			 */
			QString header(const QString& name) const;

			/** Attempt to set an HTTP header.
			 *
			 * Unlike addHeader, this will replace any existing header
			 * with the same name.
			 *
			 * @param name is the name of the header.
			 * @param value is the value to assign to the header.
			 *
			 * @returns true if the header was set.
			 * @returns false if the header could not be set (for
			 * 	example, if data has already been sent to the
			 * 	client).
			 */
			bool setHeader(const QString& name, const QString& value);

			/** Attempt to append an HTTP header.
			 *
			 * Unlike setHeader, this will not replace the header if
			 * it already is set.
			 *
			 * @param name is the name of the header.
			 * @param value is the value to assign to the header.
			 *
			 * @returns true if the header was set.
			 * @returns false if the header could not be set (for
			 * 	example, if data has already been sent to the
			 * 	client).
			 */
			bool addHeader(const QString& name, const QString& value);
			/// Give the browser a cookie.
			void setCookie(const QNetworkCookie&);
	};
}

#endif
