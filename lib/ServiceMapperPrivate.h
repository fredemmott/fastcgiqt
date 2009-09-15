#pragma once

#include "ServiceMapper.h"

#include <QObject>
#include <QMutex>

namespace FastCgiQt
{
	class ServiceMapper::Private : public QObject
	{
		Q_OBJECT;
		public:
			typedef QMap<QString, Service::Generator> ServiceMap;
			static ServiceMap services;
			static QMutex serviceLock;

			Private(ServiceMapper* parent);
		public slots:
			void cleanup(Service* service);
		private:
			ServiceMapper* m_parent;
	};
};
