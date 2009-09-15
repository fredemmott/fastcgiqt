#include <FastCgiQt/Responder.h>

class PostGreeter : public FastCgiQt::Responder
{
	Q_OBJECT;
	RESPONDER(PostGreeter);
	public:
		FASTCGIQT_RESPONDER_API_V1;
		void respond();
};
