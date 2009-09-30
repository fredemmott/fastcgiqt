#pragma once

#include "Request.h"

#include <QObject>

namespace FastCgiQt
{
	class SpawnerBase {
		public:
			virtual QObject* spawn(QObject* parent) const = 0;
	};

	template<class T> Spawner : public SpawnerBase
	{
		public:
			Spawner();
			QObject* spawn(QObject* parent) const
			{
				return new T(parent);
			}
	};

	class ThreadedResponder : public QObject
	{
		Q_OBJECT;
		public:
			ThreadedResponder(SpawnerBase* spawner, const char* responderSlot);
			~ThreadedResponder();
		public slots:
			void respond(FastCgiQt::Responder*);
		private:
			class Private;
			Private* d;
	};
};
