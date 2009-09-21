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
#ifndef _FASTCGI_QT_OUTPUT_DEVICE_H
#define _FASTCGI_QT_OUTPUT_DEVICE_H

#include <QIODevice>
#include <QHash>

namespace FastCgiQt
{
	/** @internal
	 * @brief QIODevice for returning data to the web server.
	 *
	 * This is a write-only stream device, which wraps the data in a FastCGI
	 * STDOUT record, and sends it down the socket.
	 */
	class OutputDevice : public QIODevice
	{
		Q_OBJECT
		public:
			class Backend;
			/** Create an OutputDevice.
			 *
			 * @param requestId is the FastCGI request ID.
			 * @param socket is the socket this request is using.
			 * @param parent is the parent object.
			 */
			OutputDevice(Backend* backend, QObject* parent = NULL);

			/** Whether or not to output headers.
			 *
			 * Default is true.
			 *
			 * @see setSendingHeadersEnabled
			 */
			bool isSendingHeadersEnabled() const;

			/** Set whether or not to output headers.
			 *
			 * Default is true.
			 *
			 * This must be called before any data is sent.
			 *
			 * @see isSendingHeadersEnabled
			 */
			void setSendingHeadersEnabled(bool enabled);

			/** Try (and fail) to read data from the stream.
			 *
			 * @returns -1
			 */
			qint64 readData(char* data, qint64 maxSize);

			/** Try to write data to the stream.
			 *
			 * @returns the number of bytes written, or, -1 if an
			 * 	error occured.
			 */
			qint64 writeData(const char* data, qint64 maxSize);

			/** Return a header sent/due to be sent to the client.
			 *
			 * @returns a null QString if header @p name is not set.
			 * @returns the value of header @p name otherwise.
			 */
			QString header(const QString& name) const;

			/** Try and set a header value.
			 *
			 * Unlike addHeader, this will replace any existing value
			 * for the specified header.
			 *
			 * @returns true if the header was set.
			 * @returns false if the header could not be set - for
			 * 	example, if data has already been sent to the
			 * 	client.
			 */
			bool setHeader(const QString& name, const QString& value);

			/** Try and add a header value.
			 *
			 * Unlike setHeader, this will not replace any existing
			 * value for the specified header.
			 *
			 * This will let you send two headers with the same name and
			 * differing values, but it will not let you send the same name/value
			 * pair multiple times.
			 *
			 * @returns true if the header was set.
			 * @returns false if the header could not be set - for
			 * 	example, if data has already been sent to the
			 * 	client.
			 */
			bool addHeader(const QString& name, const QString& value);

			/// Wait until all data has been sent.
			bool waitForBytesWritten(int msecs);

			enum Mode
			{
				Streamed, //< Data is written straight out, and not logged.
				Logged,   //< Data is written straight out, but a log is kept.
				Buffered  //< Not yet supported.
			};
			/** The output mode.
			 *
			 * Default is Streamed.
			 */
			Mode mode() const;
			/** Change the output mode.
			 *
			 * This can only be called before data has been written.
			 */
			void setMode(Mode mode);
			/** The output buffer.
			 *
			 * This contains logged/buffered output. It is empty if
			 * mode() == Streamed
			 */
			QByteArray buffer() const;
			/// Whether or not data has been sent on the socket yet.
			bool haveSentData() const;
		private:
			/** Output buffer/log.
			 *
			 * Only used if mode() != Streamed
			 */
			QByteArray m_buffer;
			/// If any data has been written to the socket yet.
			bool m_haveSentData;
			/// List of headers to prefix to the data.
			QHash<QString, QString> m_headers;
			/// The output mode;
			Mode m_mode;
			/// Whether or not to send headesr
			bool m_sendHeaders;

			Backend* m_backend;
	};
}

#endif
