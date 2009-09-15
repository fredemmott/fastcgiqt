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
#include "ServiceMapper.h"

#include "Service.h"
#include "ServiceMapperPrivate.h"

#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QRegExp>
#include <QStringList>
#include <QThread>

namespace FastCgiQt
{
	ServiceMapper::ServiceMapper(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent)
		:
			Responder(request, socket, inputDevice, parent),
			d(new Private(this))
	{
	}

	void ServiceMapper::addService(const QString& serviceName, Service::Generator service)
	{
		d->services.insert(serviceName, service);
	}

	void ServiceMapper::start()
	{
		{
			QMutexLocker lock(&d->serviceLock);
			if(d->services.isEmpty())
			{
				loadServices();
			}
		}

		QStringList parts = request.fullUri().split("/").filter(QRegExp("."));
		const QString serviceName(parts.isEmpty() ? "" : parts.first());

		Service* service = NULL;
		for(
			Private::ServiceMap::ConstIterator it = d->services.constBegin();
			it != d->services.constEnd();
			++it
		)
		{
			if(it.key() == serviceName || (parts.isEmpty() && it.key().isEmpty() && serviceName.isEmpty()))
			{
				if(!parts.isEmpty())
				{
					parts.takeFirst();
				}
				service = (*it.value())(request, this);
				break;
			}
		}
		
		if(!service && d->services.contains(""))
		{
			service = (d->services.value(""))(request, this);
		}

		if(service)
		{
			// copy over the ClientIOInterface parts
			// Trust me, I'm a friend class.
			service->out.setDevice(out.device());
			service->in.setDevice(in.device());

			// do the actual request
			QString fragment = parts.join("/");
			if(request.serverData("PATH_INFO").endsWith('/') && ! fragment.isEmpty())
			{
				fragment.append('/');
			}
			connect(
				service,
				SIGNAL(finished(Service*)),
				d,
				SLOT(cleanup(Service*))
			);
			service->dispatchRequest(fragment);
		}
		else
		{
			errorMessage(404);
		}
		return;
	}

	void ServiceMapper::errorMessage(int code)
	{
		setHeader("status", QString("%1 BADNESS"));

		out <<
			"<html>\n"
			"<head>\n"
			"<title>" << tr("HTTP Error %1").arg(code) << "</title>\n"
			"</head>\n"
			"<body>\n"
			"<h1>" << tr("HTTP Error %1").arg(code) << "</h1>\n"
			"<p>" << 
			tr("Error %1 occured while trying to find the page you "
			   "requested.").arg(code) << 
			"</p>\n"
			"</body>\n"
			"</html>\n"
		;
	}

	ServiceMapper::~ServiceMapper()
	{
	}
}
