#pragma once

#include "CommunicationInterface.h"

#include <QObject>

namespace FastCgiQt
{
	class CommunicationInterface::Worker : public QObject
	{
		Q_OBJECT;
		signals:
			/** This worker is done.
			 *
			 * @param inThread is the thread that this worker
			 * 	is running in.
			 */
			void finished(QThread* inThread);
		protected:
			Worker(QObject* parent);
	};
};
