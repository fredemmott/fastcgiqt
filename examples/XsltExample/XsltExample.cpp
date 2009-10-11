#include "XsltExample.h"

#include <FastCgiQt/Request.h>

XsltExample::XsltExample(QObject* parent)
: XsltService(parent)
{
}

FastCgiQt::Service::UrlMap XsltExample::urlMap() const
{
	FastCgiQt::Service::UrlMap map;

	map.append(QString(), SLOT(helloWorld()));
	return map;
}

void XsltExample::helloWorld()
{
	addXsltFile("example.xsl");
	QXmlStreamWriter out(xmlDevice());
	out.writeStartDocument();
	out.writeStartElement("dump");
	
	dumpHash(request()->rawValues(FastCgiQt::GetData), "get", &out);
	dumpHash(request()->rawValues(FastCgiQt::PostData), "post", &out);
	dumpHash(request()->rawValues(FastCgiQt::ServerData), "server", &out);

	out.writeEndElement();
	out.writeEndDocument();
}

void XsltExample::dumpHash(const QHash<QByteArray, QByteArray>& data, const QString& source, QXmlStreamWriter* xml)
{
	xml->writeStartElement(source);
	for(
		QHash<QByteArray, QByteArray>::ConstIterator it = data.constBegin();
		it != data.constEnd();
		++it
	)
	{
		xml->writeStartElement("variable");
		xml->writeTextElement("name", QString::fromLatin1(it.key()));
		xml->writeTextElement("value", QString::fromLatin1(it.value()));
		xml->writeEndElement(); // data
	}
	xml->writeEndElement();
}
