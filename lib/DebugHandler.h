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
#ifndef _FAST_CGI_QT_DEBUG_HANDLER_H
#define _FAST_CGI_QT_DEBUG_HANDLER_H

#include <QByteArray>
#include <QObject>
#include <QtGlobal>

namespace FastCgiQt
{
	/** @internal
	 * @brief Class for redirecting qDebug, qFatal, and so on to somewhere useful.
	 *
	 * This redirects to syslog, as required by the FastCGI spec.
	 */
	class DebugHandler : public QObject
	{
		public:
			/** Start redirecting messages.
			 *
			 * Also opens syslog for this process.
			 */
			DebugHandler(QObject* parent = NULL);
			/** Stop redirecting messages.
			 *
			 * Also closes sylog for this process.
			 */
			~DebugHandler();
		private:
			/// Send the specified message to syslog.
			static void syslogHandler(QtMsgType type, const char* message);

			/// The old Qt message handler.
			QtMsgHandler m_oldHandler;
	};
};

#endif
