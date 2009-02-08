#include "Service.h"

#include "ServicePrivate.h"

#include <QDebug>
#include <QGenericArgument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QStringList>

namespace FastCgiQt
{
	Service::Service(QObject* parent)
		:
			ClientIOInterface(Request(), NULL, NULL, parent)
	{
		d = new ServicePrivate();
	}

	void Service::dispatchRequest(const QString& urlFragment)
	{
		d->fillMap(this);
		Q_FOREACH(const ServicePrivate::UrlMapEntry& action, d->forwardMap)
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
			}
		}
	}

	void ServicePrivate::invokeMethod(
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

	void ServicePrivate::fillMap(Service* service)
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

		Service::UrlMap urlMap= service->urlMap();
		for(
			Service::UrlMap::ConstIterator it = urlMap.constBegin();
			it != urlMap.constEnd();
			++it
		)
		{
			QByteArray normalized = QMetaObject::normalizedSignature(it.value());
			if(!slotsToMethods.contains(normalized.constData()))
			{
				qFatal("Couldn't find a public slot matching '%s'.", normalized.constData());
			}
			///@todo check captures vs parameters
			forwardMap.append(qMakePair(QRegExp(it.key()), slotsToMethods.value(normalized)));
			Q_ASSERT(forwardMap.last().first.isValid());
		}
	}

	Service::~Service()
	{
		delete d;
	}

}
