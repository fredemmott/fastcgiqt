#include <FastCgiQt/Responder.h>

class GetGreeter : public FastCgiQt::Responder
{
	Q_OBJECT;
	RESPONDER(GetGreeter);
	public:
		void respond();
};
