#include "XsltService_Private.h"

#include "Request.h"

#include <QDebug>
#include <QTextStream>
#include <QXmlFormatter>
#include <QXmlQuery>

namespace FastCgiQt
{
	XsltService::Private::Private(FastCgiQt::Request* request)
	: source(NoXslt)
	, prettyPrint(false)
	, m_request(request)
	{
		xml.open(QIODevice::ReadWrite);
	}
	
	XsltService::Private::~Private()
	{
		if(source != NoXslt)
		{
			QXmlQuery query(QXmlQuery::XSLT20);

			xml.seek(0);
			/*
			qDebug() << "XML:" << xml.readAll();
			xml.seek(0);
			qDebug() << "XSLT:" << xslt;
			*/
	
			bool haveSetFocus = query.setFocus(&xml);
			Q_ASSERT(haveSetFocus);
			if(!haveSetFocus)
			{
				return;
			}
	
			query.bindVariable("root", QVariant::fromValue(m_request->url(RootUrl).toEncoded()));
			for(
				QVariantMap::ConstIterator it = variables.constBegin();
				it != variables.constEnd();
				++it
			)
			{
				query.bindVariable(it.key(), it.value());
			}
	
			if(m_request->responseHeader("content-type").startsWith("text/html"))
			{
				QTextStream out(m_request);
				out
					<< "<!DOCTYPE html "
					<< "PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' "
					<< "'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>"
					<< endl;
			}
	
			QBuffer xsltBuffer(&xslt);
			if(Private::FileXslt == source)
			{
				query.setQuery(xsltUrl);
			}
			else
			{
				xsltBuffer.open(QIODevice::ReadOnly);
				query.setQuery(&xsltBuffer);
			}
	
			if(prettyPrint)
			{
				QXmlFormatter formatter(query, m_request);
				query.evaluateTo(&formatter);
			}
			else
			{
				query.evaluateTo(m_request);
			}
		}
		else if(!xml.atEnd())
		{
			m_request->setHeader("content-type", "application/xml");
	
			m_request->write(xml.readAll());
		}
		m_request->write("\n");
	}
};
