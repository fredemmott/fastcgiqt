#include "ThreadPool_Worker.h"

#include <QMetaObject>
#include <QMutexLocker>

namespace FastCgiQt
{
	ThreadPool::Worker::Worker(QObject* parent)
	: QObject(parent)
	{
	}

	void ThreadPool::Worker::enqueue(QRunnable* runnable)
	{
		QMutexLocker lock(&m_queueMutex);
		m_queue.enqueue(runnable);
		staticMetaObject.invokeMethod(this, "flushQueue", Qt::QueuedConnection);
	}

	void ThreadPool::Worker::flushQueue()
	{
		QMutexLocker lock(&m_queueMutex);
		while(!m_queue.isEmpty())
		{
			QRunnable* runnable = m_queue.takeFirst();
			lock.unlock();
			runnable->run();
			if(runnable->autoDelete())
			{
				delete runnable;
			}
			lock.relock();
		}
	}
};
