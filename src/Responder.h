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
			virtual void run() = 0;
		protected:
			Responder(const Request& request, QIODevice* socket, QObject* parent = NULL);
			virtual ~Responder();

			const Request& request() const;
			QTextStream& out();
		private:
			OutputDevice* m_outputDevice;
			QTextStream m_outputStream;
			const Request m_request;
	};
	typedef Responder* (*ResponderGenerator)(const Request&, QIODevice*, QObject*);
}


#endif
