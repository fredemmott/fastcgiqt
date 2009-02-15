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
#include "DebugHandler.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>

#include <syslog.h>

namespace FastCgiQt
{
	DebugHandler::DebugHandler(QObject* parent)
		:
			QObject(parent)
	{
		qInstallMsgHandler(syslogHandler);
	}

	void DebugHandler::syslogHandler(QtMsgType type, const char* message)
	{
		QByteArray name = QCoreApplication::applicationName().toUtf8();
		::openlog(name, LOG_PERROR | LOG_PID, LOG_USER);
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
		::closelog();
	}

	DebugHandler::~DebugHandler()
	{
		qInstallMsgHandler(m_oldHandler);
	}
}
