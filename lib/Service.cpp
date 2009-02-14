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
#include "Service.h"

#include "OutputDevice.h"
#include "ServicePrivate.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QGenericArgument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QStringList>

namespace FastCgiQt
{
	// Static variables
	bool Service::Private::usingFileCache(false);
	QCache<QString, QByteArray> Service::Private::fileCache(10*1024*1024);
	QCache<QString, Service::Private::RequestCacheEntry> Service::Private::requestCache(10*1024*1024);

	Service::Service(const Request& request, QObject* parent)
		:
			ClientIOInterface(request, NULL, NULL, parent)
	{
		d = new Service::Private();
		d->dispatchingRequest = false;
	}

	QByteArray Service::readFile(const QString& path, bool useCache)
	{
		const QString prefix = path.startsWith('/') ? "" : QCoreApplication::applicationDirPath() + "/";
		const QString fullPath = prefix + path;
		if(useCache && Service::Private::fileCache.contains(fullPath))
		{
			return *Service::Private::fileCache[fullPath];
		}

		QFile file(fullPath);
		file.open(QIODevice::ReadOnly);
		Q_ASSERT(file.isOpen());

		if(this->usingFileCache() && useCache && file.size() < fileCacheSize())
		{
			Service::Private::fileCache.insert(fullPath, new QByteArray(file.readAll()), file.size());
			return *Service::Private::fileCache[fullPath];
		}
		else
		{
			return file.readAll();
		}
	}

	int Service::fileCacheSize()
	{
		return Service::Private::fileCache.maxCost();
	}

	void Service::canCacheThisRequest()
	{
		OutputDevice* outputDevice = qobject_cast<OutputDevice*>(out.device());
		Q_ASSERT(outputDevice);
		Q_ASSERT(!outputDevice->haveSentData());
		outputDevice->setMode(OutputDevice::Logged);
		d->canCacheThisRequest = true;
	}

	void Service::dispatchRequest(const QString& urlFragment)
	{
		// Don't stack-overflow if a subclass calls the wrong function
		if(d->dispatchingRequest)
		{
			qFatal(
				"Recursion detected in FastCgiQt::Service::dispatchRequest(). "
				"You probably have a subclass calling dispatchRequest which "
				"should be calling dispatchUncachedRequest instead."
			);
			return;
		}

		OutputDevice* outputDevice = qobject_cast<OutputDevice*>(out.device());
		Q_ASSERT(outputDevice);

		if(d->requestCache.contains(urlFragment))
		{
			Service::Private::RequestCacheEntry* cacheEntry = d->requestCache[urlFragment];

			if(!isExpired(urlFragment, cacheEntry->timeStamp))
			{
				outputDevice->setSendingHeadersEnabled(false);
				outputDevice->write(cacheEntry->data);
				return;
			}
		}

		d->canCacheThisRequest = false;

		d->dispatchingRequest = true;
		dispatchUncachedRequest(urlFragment);
		d->dispatchingRequest = false;

		if(d->canCacheThisRequest)
		{
			Service::Private::RequestCacheEntry* cacheEntry = new Service::Private::RequestCacheEntry();
			cacheEntry->timeStamp = QDateTime::currentDateTime();
			cacheEntry->data = outputDevice->buffer();
			d->requestCache.insert(urlFragment, cacheEntry, cacheEntry->data.length());
		}
	}

	void Service::dispatchUncachedRequest(const QString& urlFragment)
	{
		d->fillMap(this);
		Q_FOREACH(const Service::Private::UrlMapEntry& action, d->forwardMap)
		{
			QRegExp re(action.first);
			if(re.exactMatch(urlFragment))
			{
				QStringList parameters(re.capturedTexts());
				parameters.takeFirst();
				d->invokeMethod(
					this,
					action.second,
					parameters
				);
				return;
			}
		}
		///@todo FIXME
		setHeader("status", "404");

		out << "<h1>404 Not Found</h1>";
	}

	void Service::Private::invokeMethod(
		QObject* object,
		const QMetaMethod& method,
		const QStringList& parameters
	)
	{
		// Work out the method name
		QString methodName(method.signature());
		methodName = methodName.left(methodName.indexOf('('));

		// Sort out the arguments
		QList<QByteArray> parameterTypes = method.parameterTypes();
		Q_ASSERT(parameterTypes.count() == parameters.count());
		QGenericArgument arguments[10];
		for(int i = 0; i < parameters.count(); ++i)
		{
			Q_ASSERT(parameterTypes.at(i) == "QString");
			arguments[i] = Q_ARG(QString, parameters.at(i));
		}
		QMetaObject::invokeMethod(
			object,
			methodName.toLatin1().constData(),
			arguments[0],
			arguments[1],
			arguments[2],
			arguments[3],
			arguments[4],
			arguments[5],
			arguments[6],
			arguments[7],
			arguments[8],
			arguments[9]
		);
	}

	bool Service::usingFileCache()
	{
		return Service::Private::usingFileCache;
	}

	void Service::setUsingFileCache(bool useFileCache)
	{
		Service::Private::usingFileCache = useFileCache;
		if(!useFileCache)
		{
			clearFileCache();
		}
	}

	void Service::clearFileCache()
	{
		Service::Private::fileCache.clear();
	}

	void Service::setFileCacheSize(int maximumSize)
	{
		Service::Private::fileCache.setMaxCost(maximumSize);
	}

	void Service::Private::fillMap(Service* service)
	{
		if(!forwardMap.isEmpty())
		{
			return;
		}
		// Make a map of SLOT(foo) -> metamethod
		QMap<QString, QMetaMethod> slotsToMethods;
		const QMetaObject* mo = service->metaObject();
		for(
			int i = mo->methodOffset();
			i < mo->methodCount();
			++i
		)
		{
			QMetaMethod mm = mo->method(i);
			if(mm.access() != QMetaMethod::Public || mm.methodType() != QMetaMethod::Slot)
			{
				continue;
			}

			Q_ASSERT(mm.parameterTypes().count() <= 10);

			Q_FOREACH(const QString& type, mm.parameterTypes())
			{
				Q_ASSERT(type == "QString");
			}

			slotsToMethods.insert(
				QString("%1%2").arg(QSLOT_CODE).arg(mm.signature()),
				mm
			);
		}

		Service::UrlMap urlMap = service->urlMap();
		for(
			QList<QPair<QString, const char*> >::ConstIterator it = urlMap.constBegin();
			it != urlMap.constEnd();
			++it
		)
		{
			QByteArray normalized = QMetaObject::normalizedSignature(it->second);
			if(!slotsToMethods.contains(normalized.constData()))
			{
				qFatal("Couldn't find a public slot matching '%s'.", normalized.constData());
			}
			///@todo check captures vs parameters
			forwardMap.append(qMakePair(QRegExp(it->first), slotsToMethods.value(normalized)));
			Q_ASSERT(forwardMap.last().first.isValid());
		}
	}

	Service::~Service()
	{
		delete d;
	}

	int Service::requestCacheSize()
	{
		return Service::Private::requestCache.maxCost();
	}

	void Service::setRequestCacheSize(int size)
	{
		Service::Private::requestCache.setMaxCost(size);
	}

	bool Service::isExpired(const QString& urlFragment, const QDateTime& generated)
	{
		Q_UNUSED(urlFragment);
		Q_UNUSED(generated);
		return false;
	}

	void Service::UrlMap::append(const QString& regexp, const char* slot)
	{
		QList<QPair<QString, const char*> >::append(qMakePair(regexp, slot));
	}

}
