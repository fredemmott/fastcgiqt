#include <FastCgiQt/Responder.h>

class PostGreeter : public FastCgiQt::Responder
{
	Q_OBJECT;
	RESPONDER(PostGreeter);
	public:
		void respond();
};
