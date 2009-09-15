#include <FastCgiQt/Responder.h>

class GetGreeter : public FastCgiQt::Responder
{
	Q_OBJECT;
	FASTCGIQT_RESPONDER_OLD_API;
	RESPONDER(GetGreeter);
	public:
		void respond();
};
