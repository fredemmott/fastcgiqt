#include <FastCgiQt/Responder.h>

class Dumper : public FastCgiQt::Responder
{
	Q_OBJECT;
	RESPONDER(Dumper);
	public:
		void respond();
	private:
		void dumpHash(const QString& label, const QHash<QString, QString>& data);
};
