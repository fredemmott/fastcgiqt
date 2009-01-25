#include "Responder.h"

class HelloWorld : public FastCgiQt::Responder
{
	Q_OBJECT;
	RESPONDER(HelloWorld);
	public:
		void respond();
};
