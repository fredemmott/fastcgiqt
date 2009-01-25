#ifndef _FAST_CGI_QT_DEBUG_HANDLER_H
#define _FAST_CGI_QT_DEBUG_HANDLER_H

#include <QByteArray>
#include <QtGlobal>

namespace FastCgiQt
{
	class DebugHandler
	{
		public:
			DebugHandler();
			~DebugHandler();
		private:
			static void syslogHandler(QtMsgType type, const char* message);

			QtMsgHandler m_oldHandler;
			const QByteArray m_name;
	};
};

#endif
