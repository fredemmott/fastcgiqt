#include <FastCgiQt/Responder.h>

class HelloWorld : public FastCgiQt::Responder
{
	Q_OBJECT;
	FASTCGIQT_RESPONDER_API_V1;
	RESPONDER(HelloWorld);
	public:
		void respond();
};
