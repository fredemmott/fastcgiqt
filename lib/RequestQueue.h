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
#ifndef _FASTCGIQT_REQUESTQUEUE
#define _FASTCGIQT_REQUESTQUEUE

#include "AbstractResponder.h"

#include <QQueue>

class QTimer;

namespace FastCgiQt
{
	class RequestRunner;
	class RequestQueue : public QObject
	{
		Q_OBJECT
		public:
			RequestQueue(SpawnerBase* spawner, const char* responderSlot, QObject* parent);
			void respond(Request*);
			void respond(SpawnerBase* spawner, const char* responderSlot, Request* request);
		protected:
			virtual void start(RequestRunner* runner) = 0;
		private slots:
			void flushQueue();
		private:
			SpawnerBase* m_spawner;
			const char* m_responderSlot;

			QTimer* m_queueRunner;

			QQueue<RequestRunner*> m_unhandledRequests;
	};
};
#endif // _FASTCGIQT_REQUESTQUEUE
