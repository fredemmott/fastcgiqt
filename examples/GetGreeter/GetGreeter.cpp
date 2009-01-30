#include "GetGreeter.h"

#include <QString>

void GetGreeter::respond()
{
	out << "<h1>GetGreeter</h1>" << endl;
	if(request.getData("greeting").isNull())
	{
		out << QString(
			"<form action='%1' method='get'>\n"
			"<table>\n"
			"<tr><th>Name</th><td><input type='text' name='name' /></td></tr>\n"
			"<tr><th>Greeting</th><td><input type='text' name='greeting' value='Hi' /></td></tr>\n"
			"</table>\n"
			"<input type='submit' />\n"
			"</form>\n"
		).arg(request.serverData("SCRIPT_NAME"));
	}
	else
	{
		out << QString("%1, %2").arg(request.getData("greeting")).arg(request.getData("name"));
	}
}
