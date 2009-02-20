#ifndef _FASTCGI_QT_DATABASE_GUARD_H
#define _FASTCGI_QT_DATABASE_GUARD_H
#include <QSqlDatabase>

namespace FastCgiQt
{
	class DatabaseGuard
	{
		public:
			DatabaseGuard(const QString& connectionName);
			~DatabaseGuard();
		private:
			const QString m_database;
	};
}

#endif
