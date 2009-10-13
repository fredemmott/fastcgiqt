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
