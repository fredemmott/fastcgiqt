#pragma once

#include <QQueue>
#include <QRunnable>
#include <QThread>

namespace FastCgiQt
{
	/// @internal Like QThreadPool, but you can specify an object to be move to the worker thread.
	class ThreadPool : public QObject
	{
		Q_OBJECT;
		public:
			/// Construct a ThreadPool
			ThreadPool(QObject* parent = 0);
			~ThreadPool();
			/** Queue a job in a worker thread, with a payload for that thread.
			 *
			 * The @p payload will be moved to the worker thread.
			 */
			void start(QRunnable* runnable, QObject* payload);
		private:
			class Worker;

			int m_nextWorker;
			const int m_threadCount;
			QThread** m_threads;
			Worker** m_workers;
	};
};
