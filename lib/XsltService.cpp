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

#include <QCoreApplication>
#include <QDebug>
#include <QFile>

namespace FastCgiQt
{
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
