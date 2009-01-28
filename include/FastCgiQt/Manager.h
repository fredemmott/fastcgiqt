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
			Manager(ResponderGenerator responderGenerator, QObject* parent = NULL);
			/// Destroy the Manager.
			~Manager();
		private:
			///@internal
			ManagerPrivate* d;
	};
}

#endif
