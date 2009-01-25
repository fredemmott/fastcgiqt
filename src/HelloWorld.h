#include "Responder.h"

class HelloWorld : public FastCgiQt::Responder
{
	Q_OBJECT;
	RESPONDER(HelloWorld);
	public:
		void respond();
	private:
		void dumpHash(const QString& label, const QHash<QString, QString>& data);
};
