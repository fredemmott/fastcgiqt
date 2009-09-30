#include "ThreadPool.h"
#include "ThreadPool_Worker.h"

#include <QDebug>

namespace FastCgiQt
{
	ThreadPool::ThreadPool(QObject* parent)
	: QObject(parent)
	, m_nextWorker(0)
	, m_threadCount(QThread::idealThreadCount())
	, m_threads(new QThread*[m_threadCount])
	, m_workers(new Worker*[m_threadCount])
	{
		for(int i = 0; i < m_threadCount; ++i)
		{
			QThread* thread = new QThread(this);
			m_threads[i] = thread;
			thread->start();
			Worker* worker = new Worker(0);
			worker->moveToThread(thread);
			m_workers[i] = worker;
		}
	}

	ThreadPool::~ThreadPool()
	{
		for(int i = 0; i < m_threadCount; ++i)
		{
			delete m_threads[i];
			delete m_workers[i];
		}
		delete[] m_workers;
		delete[] m_threads;
	}

	void ThreadPool::start(QRunnable* runnable, QObject* payload)
	{
		Worker* worker = m_workers[m_nextWorker];
		payload->moveToThread(worker->thread());
		worker->enqueue(runnable);
		m_nextWorker = (m_nextWorker + 1) % m_threadCount;
	}
};
