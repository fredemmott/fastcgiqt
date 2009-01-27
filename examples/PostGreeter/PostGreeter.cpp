#include "PostGreeter.h"

#include <QString>

void PostGreeter::respond()
{
	out << "Content-type: text/html\r\n\r\n" << flush;
	out << "<h1>PostGreeter</h1>" << endl;
	if(request.postData("greeting").isNull())
	{
		out << QString(
			"<form action='%1' method='post'>\n"
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
		out << QString("%1, %2").arg(request.postData("greeting")).arg(request.postData("name"));
	}
}
