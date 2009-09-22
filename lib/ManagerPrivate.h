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
#ifndef _FASTCGI_QT_MANAGER_PRIVATE_H
#define _FASTCGI_QT_MANAGER_PRIVATE_H

#include "Caches.h"
#include "CommunicationInterface.h"
#include "Responder.h"

#include <QObject>

class QFileSystemWatcher;

namespace FastCgiQt
{
	/** @internal
	 * @brief Class doing main application setup.
	 *
	 * This sets up caches, and sets up a CommunicationInterface, such
	 * as FastCgiInterface.
	 */
	class ManagerPrivate : public QObject
	{
		Q_OBJECT
		public:
			/// Create a ManagerPrivate object.
			ManagerPrivate(Responder::Generator responderGenerator, QObject* parent = NULL);
			~ManagerPrivate();
		private slots:
			/// Request that the application shuts down.
			void shutdown();
		private:
			/// Show CLI for configuring the way FastCgiQt communicates with the web server
			void configureHttpd();
			/// Show CLI for configuring the database
			void configureDatabase();

			/// Watcher to call shutdown() if the executable is modified.
			QFileSystemWatcher* m_applicationWatcher;

			/// Scope guard to create and cleanup global caches
			Caches* m_caches;

			CommunicationInterface* m_interface;

			QList<CommunicationInterface::Factory*> m_factories;
	};
};

#endif
