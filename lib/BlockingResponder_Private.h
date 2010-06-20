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
#ifndef _FASTCGIQT_BLOCKINGRESPONDER_PRIVATE
#define _FASTCGIQT_BLOCKINGRESPONDER_PRIVATE

#include "BlockingResponder.h"
#include "RequestQueue.h"

#include <QQueue>

namespace FastCgiQt
{
	class BlockingResponder::Private : public RequestQueue
	{
		Q_OBJECT;
		public:
			Private(SpawnerBase* spawner, const char* responderSlot, QObject* parent);
		protected:
			void start(RequestRunner* runner);
	};
};
#endif // _FASTCGIQT_BLOCKINGRESPONDER_PRIVATE
