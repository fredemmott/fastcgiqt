/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include "PostGreeter.h"

#include <QString>

void PostGreeter::respond()
{
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
