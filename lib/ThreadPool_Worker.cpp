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
