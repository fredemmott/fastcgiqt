#include <FastCgiQt/Responder.h>

class GetGreeter : public FastCgiQt::Responder
{
	Q_OBJECT;
	FASTCGIQT_RESPONDER_API_V1;
	RESPONDER(GetGreeter);
	public:
		void respond();
};
