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
			static void removeConnection();
			static QString connectionName();
			static int threadId();

			static QAtomicInt m_maxThreadId;
			static QThreadStorage<int*> m_threadIds;
			static QThreadStorage<DatabaseGuard*> m_databases;
	};
}

#endif
