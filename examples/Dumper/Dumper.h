#include <FastCgiQt/Responder.h>

class Dumper : public FastCgiQt::Responder
{
	Q_OBJECT;
	RESPONDER(Dumper);
	public:
		FASTCGIQT_RESPONDER_OLD_API;
		void respond();
	private:
		void dumpHash(const QString& label, const QHash<QString, QString>& data);
};
