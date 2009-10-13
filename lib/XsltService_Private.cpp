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
