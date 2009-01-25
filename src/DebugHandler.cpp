#include "DebugHandler.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>

#include <syslog.h>

namespace FastCgiQt
{
	DebugHandler::DebugHandler()
		:
			m_name(QCoreApplication::applicationName().toUtf8())
	{
		::openlog(m_name.constData(), /*LOG_PERROR |*/ LOG_PID, LOG_USER);
		qInstallMsgHandler(syslogHandler);
	}

	void DebugHandler::syslogHandler(QtMsgType type, const char* message)
	{
		switch(type)
		{
			case QtDebugMsg:
				::syslog(LOG_DEBUG, "DEBUG: %s", message);
				break;
			case QtWarningMsg:
				::syslog(LOG_WARNING, "WARNING: %s", message);
				break;
			case QtCriticalMsg:
				::syslog(LOG_CRIT, "CRITICAL: %s", message);
				break;
			case QtFatalMsg:
				::syslog(LOG_EMERG, "FATAL: %s", message);
				abort();
			default:
				::syslog(LOG_EMERG, "UNKNOWN: %s", message);
				abort();
		}
	}

	DebugHandler::~DebugHandler()
	{
		::closelog();
		qInstallMsgHandler(m_oldHandler);
	}
}
