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
#ifndef _FASTCGI_QT_MANAGER_H
#define _FASTCGI_QT_MANAGER_H

#include "Responder.h"

#include <QObject>

namespace FastCgiQt
{
	class ManagerPrivate;
	/** Class managing FastCGI connections.
	 *
	 * This class is responsible for listening to the FastCGI socket,
	 * and acting on whatever data is received.
	 * 
	 * This includes spawning Responder classes.
	 *
	 * @ingroup core
	 */
	class Manager : public QObject
	{
		Q_OBJECT
		public:
			/** Create a Manager.
			 *
			 * @param responderGenerator is a pointer to a function creating Responder objects.
			 * @param parent is the parent object.
			 */
			Manager(Responder::Generator responderGenerator, QObject* parent = NULL);
			/// Destroy the Manager.
			~Manager();
		private:
			///@internal
			ManagerPrivate* d;
	};
}

#endif
