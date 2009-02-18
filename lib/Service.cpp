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
#include <QReadLocker>
#include <QMetaMethod>
#include <QMetaObject>
#include <QStringList>
#include <QThread>
#include <QXmlStreamWriter>

namespace FastCgiQt
{
	// Static variables
	bool Service::Private::usingFileCache(false);
	FileCache Service::Private::fileCache(10*1024*1024);
	FileDependentCache Service::Private::requestCache(10*1024*1024);

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

		if(d->canCacheThisRequest)
		{
			// not if use-cache:
			// if caching of this file isn't wanted, then surely the
			// request cache should be invalidated here too
			d->requestCache.addDependency(d->urlFragment, fullPath);
		}

		if(useCache && d->fileCache.contains(fullPath))
		{
			return d->fileCache[fullPath]->data;
		}

		QFile file(fullPath);
		file.open(QIODevice::ReadOnly);
		Q_ASSERT(file.isOpen());

		if(this->usingFileCache() && useCache && file.size() < fileCacheSize())
		{
			CacheEntry* entry = new CacheEntry(QDateTime::currentDateTime(), file.readAll());
			QWriteLocker lock(d->fileCache.readWriteLock());
			d->fileCache.insert(fullPath, entry);
			return entry->data;
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

	void Service::cacheThisRequest()
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

		{
			QReadLocker lock(d->fileCache.readWriteLock());
			if(d->requestCache.contains(urlFragment))
			{
				CacheEntry* cacheEntry = d->requestCache[urlFragment];
	
				if(!isExpired(urlFragment, cacheEntry->timeStamp))
				{
					outputDevice->setSendingHeadersEnabled(false);
					outputDevice->write(cacheEntry->data);
					return;
				}
			}
		}

		d->urlFragment = urlFragment;

		d->canCacheThisRequest = false;

		d->dispatchingRequest = true;
		dispatchUncachedRequest(urlFragment);
		d->dispatchingRequest = false;

		if(d->canCacheThisRequest)
		{
			CacheEntry* cacheEntry = new CacheEntry(QDateTime::currentDateTime(), outputDevice->buffer());
			d->requestCache.insert(urlFragment, cacheEntry);
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

	void Service::dumpCacheInformation()
	{
		QReadLocker fileLock(d->fileCache.readWriteLock());
		QReadLocker requestLock(d->requestCache.readWriteLock());
		QXmlStreamWriter xml(out.device());
		xml.writeStartDocument();
		xml.writeStartElement("html");
			xml.writeStartElement("head");
				xml.writeTextElement("title", tr("Cache Information"));
			xml.writeEndElement();
			xml.writeStartElement("body");
				xml.writeTextElement("h1", tr("Cache Information"));
				xml.writeStartElement("dl");
					xml.writeTextElement("dt", tr("File cache size:"));
					xml.writeTextElement("dd", tr("%1 bytes").arg(d->fileCache.totalCost()));
					xml.writeTextElement("dt", tr("Maximum file cache size:"));
					xml.writeTextElement("dd", tr("%1 bytes").arg(d->fileCache.maxCost()));
					xml.writeTextElement("dt", tr("Items in file cache:"));
					xml.writeStartElement("dd");
						xml.writeCharacters(tr("%1 files:").arg(d->fileCache.count()));
						xml.writeStartElement("ul");
							Q_FOREACH(const QString& file, d->fileCache.keys())
							{
								xml.writeTextElement("li", QString("'%1' (%2 bytes)").arg(file).arg(d->fileCache[file]->data.length()));
							}
						xml.writeEndElement();
					xml.writeEndElement();
					xml.writeTextElement("dt", tr("Request cache size:"));
					xml.writeTextElement("dd", tr("%1 bytes").arg(d->requestCache.totalCost()));
					xml.writeTextElement("dt", tr("Maximum equest cache size:"));
					xml.writeTextElement("dd", tr("%1 bytes").arg(d->requestCache.maxCost()));
					xml.writeTextElement("dt", tr("Items in request cache:"));
					xml.writeStartElement("dd");
						xml.writeCharacters(tr("%1 pages:").arg(d->requestCache.count()));
						xml.writeStartElement("ul");
							Q_FOREACH(const QString& urlFragment, d->requestCache.keys())
							{
								xml.writeStartElement("li");
									xml.writeCharacters(QString("'%1' (%2 bytes)").arg(urlFragment).arg(d->requestCache[urlFragment]->data.length()));
									xml.writeTextElement("p", tr("File dependencies:"));
									xml.writeStartElement("ul");
										Q_FOREACH(const QString& file, d->requestCache.dependencies(urlFragment))
										{
											xml.writeTextElement("li", file);
										}
									xml.writeEndElement();
								xml.writeEndElement();
							}
						xml.writeEndElement();
					xml.writeEndElement();
				xml.writeEndElement();
			xml.writeEndElement();
		xml.writeEndElement();
		xml.writeEndDocument();
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
			int i = Service::staticMetaObject.methodOffset();
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
