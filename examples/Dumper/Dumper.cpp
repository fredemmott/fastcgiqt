#include "Dumper.h"

void Dumper::respond()
{
	out << "Content-type: text/html\r\n\r\n" << flush;
	out << "<h1>" << tr("Variable Dump") << "</h1>";
	dumpHash(tr("Get variables"), request.getData());
	dumpHash(tr("Post variables"), request.postData());
	dumpHash(tr("Server variables"), request.serverData());
}

void Dumper::dumpHash(const QString& label, const QHash<QString, QString>& data)
{
	// Print out server variables
	out << "<h2>" << label << "</h2>" << endl;

	out << "<table>" << endl;

	for(
		QHash<QString, QString>::ConstIterator it = data.constBegin();
		it != data.constEnd();
		++it
	)
	{
		out << "<tr><th>" << it.key() << "</th><td>" << it.value() << "</td></tr>" << endl;
	}

	out << "</table>" << endl;

}
