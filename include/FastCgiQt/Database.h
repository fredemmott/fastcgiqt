#ifndef _FASTCGI_QT_DATABASE_H
#define _FASTCGI_QT_DATABASE_H

#include <QSqlDatabase>

namespace FastCgiQt
{
	/** Convenience class for using and cleaning up a single database connection per application.
	 *
	 * Database connections will be removed and cleaned up when the calling thread exits.
	 */
	class Database
	{
		public:
			static bool haveConnection();
			static QSqlDatabase database(bool open = true);
			static QSqlDatabase addDatabase(const QString& type);
	};
}

#endif
