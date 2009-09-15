#include "ServiceMapperPrivate.h"

#include <QDebug>

namespace FastCgiQt
{
	ServiceMapper::Private::ServiceMap ServiceMapper::Private::services;
	QMutex ServiceMapper::Private::serviceLock;

	ServiceMapper::Private::Private(ServiceMapper* parent)
	: QObject(parent)
	, m_parent(parent)
	{
		Q_ASSERT(parent);
	}

	void ServiceMapper::Private::cleanup(Service* service)
	{
		service->out << flush;
		delete service;
		m_parent->finished();
	}
};
