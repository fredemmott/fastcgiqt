#ifndef _FASTCGI_QT_RESPONDER_H
#define _FASTCGI_QT_RESPONDER_H

#include "Request.h"

#include <QTextStream>

class QIODevice;

namespace FastCgiQt
{
	class Responder : public QObject
	{
		Q_OBJECT
		public:
			virtual void respond() = 0;
			virtual ~Responder();
		protected:
			Responder(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent = NULL);

			const Request& request;
			QTextStream in;
			QTextStream out;
	};
	typedef Responder* (*ResponderGenerator)(const Request&, QIODevice*, QIODevice* inputDevice, QObject*);
}

#define RESPONDER(x) \
	public: \
		x(const FastCgiQt::Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent = NULL) \
			: Responder(request, socket, inputDevice, parent) {} \
		static Responder* instance(const FastCgiQt::Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent) \
			{ return new x(request, socket, inputDevice, parent); }
#endif
