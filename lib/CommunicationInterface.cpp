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
#include "CommunicationInterface.h"
#include "CommunicationInterface_Worker.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

namespace FastCgiQt
{
	CommunicationInterface::CommunicationInterface(QObject* parent)
	: QObject(parent)
	{
		// Spawn some threads
		for(int i = 0; i < qMax(QThread::idealThreadCount(), 1); ++i)
		{
			QThread* thread = new QThread(this);
			thread->start();
			m_threads.append(thread);
			m_threadLoads[thread] = 0;
		}
	}

	void CommunicationInterface::exitIfFinished()
	{
		if(!isFinished())
		{
			return;
		}
		qDebug() << "Waiting for threads - thread loads:" << threadLoads();
		Q_FOREACH(QThread* thread, m_threads)
		{
			if(m_threadLoads.value(thread) != 0)
			{
				return;
			}
		}
		qDebug() << "Shutting down threads";
		Q_FOREACH(QThread* thread, m_threads)
		{
			thread->quit();
			bool done = thread->wait(10000);
			if(!done)
			{
				qDebug() << "One thread took longer than 10 seconds to shut down, terminating";
				thread->terminate();
			}
		}
		qDebug() << "Shutdown complete. No thread load.";
		QCoreApplication::exit();
	}

	QList<int> CommunicationInterface::threadLoads() const
	{
		QList<int> data;
		Q_FOREACH(const QAtomicInt& load, m_threadLoads)
		{
			data.append(load);
		}
		return data;
	}


	CommunicationInterface::~CommunicationInterface()
	{
	}

	bool CommunicationInterface::hasLessLoadThan(QThread* t1, QThread* t2)
	{
		Q_ASSERT(t1->parent() == t2->parent());
		CommunicationInterface* p = qobject_cast<CommunicationInterface*>(t1->parent());
		Q_ASSERT(p);
		return p->m_threadLoads.value(t1) < p->m_threadLoads.value(t2);
	}

	void CommunicationInterface::addWorker(Worker* worker)
	{
		// Pick a thread to put it in
		qSort(m_threads.begin(), m_threads.end(), hasLessLoadThan);
		QThread* thread = m_threads.first();
		m_threadLoads[thread].ref();

		// Deref when finished
		connect(
			worker,
			SIGNAL(finished(QThread*)),
			this,
			SLOT(reduceLoadCount(QThread*))
		);
		// Move it to a thread
		worker->moveToThread(thread);
	}

	void CommunicationInterface::reduceLoadCount(QThread* thread)
	{
		m_threadLoads[thread].deref();
		exitIfFinished();
	}
};
