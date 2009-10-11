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
#include "XsltService.h"
#include "XsltService_Private.h"

#include "Request.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
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
	
	XsltService::XsltService(QObject* parent)
	: Service(parent)
	, d(0)
	{
	}

	void XsltService::dispatchUncachedRequest(const QByteArray& suffix)
	{
		d = new Private(request());
		Service::dispatchUncachedRequest(suffix);
		if(!request()->parent())
		{
			delete d;
			d = 0;
		}
	}
	
	XsltService::~XsltService()
	{
		delete d;
	}

	XsltService::Private::~Private()
	{
		if(source != NoXslt)
		{
			QXmlQuery query(QXmlQuery::XSLT20);

			xml.seek(0);
	
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
	}
	
	void XsltService::setVariable(const QString& name, const QVariant& value)
	{
		d->variables[name] = value;
	}
	
	void XsltService::setXsltUrl(const QString& path)
	{
		if(path.isEmpty())
		{
			setXsltUrl(QUrl());
		}
		else
		{
			setXsltUrl(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/" + path));
		}
	}
	
	void XsltService::addXsltFile(const QString& fileName)
	{
		d->source = Private::StringXslt;
		///@fixme cache
		QFile file(fileName);
		file.open(QIODevice::ReadOnly);
		d->xslt += file.readAll();
	}
	
	void XsltService::setXsltUrl(const QUrl& url)
	{
		d->xsltUrl = url;
		if(url.isValid())
		{
			d->source = Private::FileXslt;
		}
		else
		{
			d->source = Private::NoXslt;
		}
	}
	
	void XsltService::setPrettyPrintingEnabled(bool value)
	{
		d->prettyPrint = value;
	}
	
	bool XsltService::isPrettyPrintingEnabled() const
	{
		return d->prettyPrint;
	}
}
