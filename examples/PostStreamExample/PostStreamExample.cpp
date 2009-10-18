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
#include "PostStreamExample.h"

#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QTextStream>

/* WARNING
 * -------
 * 
 * This creates the output HTML in an incredibly insecure manner, and is vulnerable to XSS.
 *
 * This is demonstrating the stream reader only, not good ways to output HTML.
 *
 * See XsltService for a nicer output method.
 */

using FastCgiQt::PostDataStreamReader;

PostStreamExample::PostStreamExample(QObject* parent)
: QObject(parent)
, m_request(0)
, m_streamReader(0)
{
}

PostStreamExample::~PostStreamExample()
{
	delete m_streamReader;
	delete m_request;
}

void PostStreamExample::respond(FastCgiQt::Request* request)
{
	QTextStream out(request);
	out << "<h1>PostStreamExample</h1>" << endl;
	m_request = request;
	request->setParent(this); // Take ownership of the request; this means that it's up to this object to delete itself (and the request)

	out << QString(
		"<form action='%1' method='post'>\n"
		"<table>\n"
		"<tr><th>Name</th><td><input type='text' name='name' /></td></tr>\n"
		"<tr><th>Greeting</th><td><input type='text' name='greeting' value='Hi' /></td></tr>\n"
		"</table>\n"
		"<input type='submit' />\n"
		"</form>\n"
	).arg(QLatin1String(request->url(FastCgiQt::LocationUrl).toEncoded()));

	m_streamReader = new PostDataStreamReader(request);

	connect(
		request,
		SIGNAL(readyRead()),
		this,
		SLOT(readNext())
	);
	out.flush(); // Flush the stream, otherwise, we might get some output created by readNext() above some of the output already created
	readNext();
}

void PostStreamExample::readNext()
{
	QTextStream out(m_request);
	PostDataStreamReader& reader = *m_streamReader; // convenience

	reader.readNext();
	switch(reader.tokenType())
	{
		case PostDataStreamReader::StartData:
			out << "<h2>Posted variables</h2>" << endl;
			out << "<dl>" << endl;
			break;
		case PostDataStreamReader::VariableName:
			out << "<dn>" << reader.variableName() << "</dn>" << endl;
			break;
		case PostDataStreamReader::VariableValue:
			out << "<dd>" << reader.variableValue() << "</dd>" << endl;
			break;
		case PostDataStreamReader::EndData:
			out << "</dl>" << endl;
			deleteLater(); // Delete this object (and the request) from the Qt event loop, finishing the response
			return; // Don't recurse
		case PostDataStreamReader::Invalid:
			return; // Don't recurse
		default:
			break;
	}
	readNext();
}
