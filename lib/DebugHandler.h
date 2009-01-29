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
			/** The name of the application.
			 *
			 * This is taken from QCoreApplication::applicationName().utf8().
			 *
			 * A copy is made, as syslog wants a const char* to it.
			 */
			const QByteArray m_name;
	};
};

#endif
