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
#ifndef _FASTCGIQT_MANAGER
#define _FASTCGIQT_MANAGER

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>

namespace FastCgiQt
{
	class Request;

	/** Class managing FastCGI connections.
	 *
	 * This class is responsible for listening to the FastCGI socket,
	 * and acting on whatever data is received.
	 *
	 * @ingroup core
	 */
	class Manager : public QObject
	{
		Q_OBJECT
		public:
			/** Create a Manager.
			 *
			 * This manager will read its' configuration files.
			 *
			 * @param parent is the parent object.
			 */
			Manager(QObject* parent = 0);

			/** Create a Manager with the specified configuration.
			 *
			 * This should really only be used for embedding, or if you provide your own configuration interface.
			 * It reduces flexibility.
			 *
			 * @param backend is the backend to use (eg 'HTTP', 'FCGI-UNIX', and so on).
			 * @param configuration is a key->QVariant mapping of configuration values that would be in the 'FastCgiQt' section of the configuration file.
			 * @param parent is the parent QObject.
			 */
			Manager(const char* backend, const QMap<QString, QVariant>& configuration, QObject* parent = 0);
			/// Destroy the Manager.
			~Manager();
		signals:
			void newRequest(FastCgiQt::Request*);
		private:
			class Private;
			///@internal
			Private* d;
	};
}
#endif // _FASTCGIQT_MANAGER
