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
#include "ThreadPool.h"

#include "Settings.h"
#include "ThreadPool_Worker.h"

#include <QDebug>

namespace FastCgiQt
{
	ThreadPool::ThreadPool(QObject* parent)
	: QObject(parent)
	, m_nextWorker(0)
	, m_threadCount(-1)
	, m_threads(0)
	, m_workers(0)
	{
		Settings settings;
		const int threadSetting = settings.value("FastCGI/numberOfThreads", -1).toInt();
		if(threadSetting == -1)
		{
			m_threadCount = QThread::idealThreadCount();
		}
		else
		{
			Q_ASSERT(threadSetting > 0);
			m_threadCount = threadSetting;
		}

		Q_ASSERT(m_threadCount > 0);

		m_threads = new QThread*[m_threadCount];
		m_workers = new Worker*[m_threadCount];

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
			m_threads[i]->quit();
			m_threads[i]->wait(10000); // 10 seconds
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
