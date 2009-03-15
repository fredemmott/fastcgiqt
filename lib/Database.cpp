#include "Database.h"
#include "DatabasePrivate.h"

#include <QSqlError>
#include <QSqlQuery>
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

		if(!sqlThinksSo)
		{
			// Nothing indicates it's open, it's not
			return false;
		}
		// Just because we /had/ a connection doesn't mean it's still open
		if(!database().isOpen())
		{
			removeConnection();
			return false;
		}
		if(sqlThinksSo)
		{
			QSqlQuery query(database());
			query.exec("SELECT 1");
			if(query.lastError().isValid())
			{
				removeConnection();
				return false;
			}
		}
		return true;
	}

	void DatabasePrivate::removeConnection()
	{
		m_databases.setLocalData(NULL);
		Q_ASSERT(!m_databases.hasLocalData());
		QSqlDatabase::removeDatabase(connectionName());
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
