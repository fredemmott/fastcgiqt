#include <FastCgiQt/Responder.h>

class HelloWorld : public FastCgiQt::Responder
{
	Q_OBJECT;
	FASTCGIQT_RESPONDER_OLD_API;
	RESPONDER(HelloWorld);
	public:
		void respond();
};
