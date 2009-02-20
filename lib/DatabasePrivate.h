#ifndef _FASTCGI_QT_DATABASE_PRIVATE_H
#define _FASTCGI_QT_DATABASE_PRIVATE_H

#include "DatabaseGuard.h"

#include <QAtomicInt>
#include <QThreadStorage>

class QThread;

namespace FastCgiQt
{
	class DatabasePrivate
	{
		public:
			static QSqlDatabase database(bool open = true);
			static QSqlDatabase addDatabase(const QString& type);
			static bool haveConnection();
		private:
			static QString connectionName();
			static int threadId();

			static QAtomicInt m_maxThreadId;
			static QThreadStorage<int*> m_threadIds;
			static QThreadStorage<DatabaseGuard*> m_databases;
	};
}

#endif
