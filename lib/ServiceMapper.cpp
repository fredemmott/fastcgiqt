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

#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QThread>

namespace FastCgiQt
{
	class ServiceMapper::Private
	{
		public:
			typedef QMap<QString, Service::Generator> ServiceMap;
			static QMap<QThread*, ServiceMap> services;
	};
	QMap<QThread*, ServiceMapper::Private::ServiceMap> ServiceMapper::Private::services;

	ServiceMapper::ServiceMapper(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent)
		:
			Responder(request, socket, inputDevice, parent),
			d(new Private)
	{
	}

	void ServiceMapper::addService(const QString& serviceName, Service::Generator service)
	{
		d->services[QThread::currentThread()].insert(serviceName, service);
	}

	void ServiceMapper::respond()
	{
		if(d->services.value(QThread::currentThread()).isEmpty())
		{
			loadServices();
		}
		Private::ServiceMap services = d->services.value(QThread::currentThread());

		QStringList parts = request.serverData("PATH_INFO").split("/").filter(QRegExp("."));;
		const QString serviceName(parts.isEmpty() ? "" : parts.takeFirst());

		for(
			Private::ServiceMap::ConstIterator it = services.constBegin();
			it != services.constEnd();
			++it
		)
		{
			if(it.key() == serviceName || (parts.isEmpty() && it.key().isEmpty() && serviceName.isEmpty()))
			{
				Service* service = (*it.value())(this);

				// copy over the ClientIOInterface parts
				// Trust me, I'm a friend class.
				*const_cast<Request*>(&service->request) = *const_cast<Request*>(&this->request);
				service->out.setDevice(out.device());
				service->in.setDevice(in.device());

				// do the actual request
				service->dispatchRequest(parts.join("/"));

				// flush the text stream
				service->out << flush;
				delete service;
				return;
			}
		}
		errorMessage(404);
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
		delete d;
	}
}
