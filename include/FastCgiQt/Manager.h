#ifndef _FASTCGI_QT_MANAGER_H
#define _FASTCGI_QT_MANAGER_H

#include "Responder.h"

#include <QObject>

namespace FastCgiQt
{
	class ManagerPrivate;
	class Manager : public QObject
	{
		Q_OBJECT
		public:
			Manager(ResponderGenerator responderGenerator, QObject* parent = NULL);
			~Manager();
		private:
			ManagerPrivate* d;
	};
}

#endif
