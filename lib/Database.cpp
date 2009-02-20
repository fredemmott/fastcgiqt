#include "Database.h"
#include "DatabasePrivate.h"

#include <QStringList>
#include <QThread>

namespace FastCgiQt
{
	QAtomicInt DatabasePrivate::m_maxThreadId;
	QThreadStorage<int*> DatabasePrivate::m_threadIds;
	QThreadStorage<DatabaseGuard*> DatabasePrivate::m_databases;

	QSqlDatabase Database::database(bool open)
	{
		return DatabasePrivate::database(open);
	}

	QSqlDatabase Database::addDatabase(const QString& type)
	{
		return DatabasePrivate::addDatabase(type);
	}

	bool Database::haveConnection()
	{
		return DatabasePrivate::haveConnection();
	}

	bool DatabasePrivate::haveConnection()
	{
		bool sqlThinksSo = QSqlDatabase::connectionNames().contains(connectionName());
		bool localStorageThinksSo = m_databases.hasLocalData();
		Q_ASSERT(sqlThinksSo == localStorageThinksSo);
		return sqlThinksSo;
	}

	int DatabasePrivate::threadId()
	{
		int threadId;
		if(!m_threadIds.hasLocalData())
		{
			threadId = m_maxThreadId.fetchAndAddRelease(1);
			m_threadIds.setLocalData(new int(threadId));
		}
		else
		{
			threadId = *m_threadIds.localData();
		}
		return threadId;
	}

	QString DatabasePrivate::connectionName()
	{
		return QString("FastCgiQt::Database connection - thread %1").arg(threadId());
	}

	QSqlDatabase DatabasePrivate::database(bool open)
	{
		return QSqlDatabase::database(connectionName(), open);
	}

	QSqlDatabase DatabasePrivate::addDatabase(const QString& type)
	{
		if(!m_databases.hasLocalData())
		{
			m_databases.setLocalData(new DatabaseGuard(connectionName()));
		}
		return QSqlDatabase::addDatabase(type, connectionName());
	}
}
