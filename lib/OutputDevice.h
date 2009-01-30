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
			/** Create an OutputDevice.
			 *
			 * @param requestId is the FastCGI request ID.
			 * @param socket is the socket this request is using.
			 * @param parent is the parent object.
			 */
			OutputDevice(quint16 requestId, QIODevice* socket, QObject* parent = NULL);

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
		private:
			/// The FastCGI request ID.
			quint16 m_requestId;
			/// The socket that output should be sent on.
			QIODevice* m_socket;
	};
}

#endif
