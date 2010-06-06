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
#include "Dumper.h"

#include <QByteArray>
#include <QHash>
#include <QTextStream>

using namespace FastCgiQt;

Dumper::Dumper(QObject* parent)
: QObject(parent)
{
}

void Dumper::respond(Request* request)
{
	QTextStream out(request);
	out << "<h1>" << tr("Variable Dump") << "</h1>";
	out << "<dl>" << endl;
	out << "<dt>Root URL:</dt><dd>" << request->url(RootUrl).toEncoded() << "</dd>" << endl;
	out << "<dt>Location URL:</dt><dd>" << request->url(LocationUrl).toEncoded() << "</dd>" << endl;
	out << "<dt>Full URL:</dt><dd>" << request->url(RequestUrl).toEncoded() << "</dd>" << endl;
	out << "</dl>" << endl;
	dumpHash(tr("Get variables"), request->rawValues(GetData), out);
	dumpHash(tr("Post variables"), request->rawValues(PostData), out);
	dumpHash(tr("Server variables"), request->rawValues(ServerData), out);
}

void Dumper::dumpHash(const QString& label, const QHash<QByteArray, QByteArray>& data, QTextStream& out)
{
	// Print out server variables
	out << "<h2>" << label << "</h2>" << endl;

	out << "<table>" << endl;

	QList<QByteArray> keys = data.keys();
	qSort(keys);

	Q_FOREACH(const QByteArray& key, keys)
	{
		QList<QByteArray> values = data.values(key);
		out << "<tr><th rowspan='" << values.count() << "'>" << key << "</th>";
		Q_FOREVER
		{
			const QByteArray value = values.takeFirst();
			out << "<td>" << value << "</td></tr>" << endl;
			if(!values.isEmpty())
			{
				out << "<tr>";
			}
			else
			{
				break;
			}
		}
	}

	out << "</table>" << endl;

}
