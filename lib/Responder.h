#ifndef _FASTCGI_QT_RESPONDER_H
#define _FASTCGI_QT_RESPONDER_H

#include "Request.h"

#include <QTextStream>

class QIODevice;

namespace FastCgiQt
{
	class OutputDevice;

	class Responder : public QObject
	{
		Q_OBJECT
		public:
			virtual void respond() = 0;
			virtual ~Responder();
		private:
			OutputDevice* m_outputDevice;
		protected:
			Responder(const Request& request, QIODevice* socket, QObject* parent = NULL);

			const Request& request;
			QTextStream out;
	};
	typedef Responder* (*ResponderGenerator)(const Request&, QIODevice*, QObject*);
}

#define RESPONDER(x) \
	public: \
		x(const FastCgiQt::Request& request, QIODevice* socket, QObject* parent = NULL) \
			: Responder(request, socket, parent) {} \
		static Responder* instance(const FastCgiQt::Request& request, QIODevice* socket, QObject* parent) \
			{ return new x(request, socket, parent); }
#endif
