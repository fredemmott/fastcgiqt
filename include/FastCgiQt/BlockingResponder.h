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

#include "AbstractResponder.h"

namespace FastCgiQt
{
	/** Single-threaded dispatcher for FastCgiQt requests.
	 *
	 * @section advantages_direct Advantages over using Manager::newRequest directly
	 *  - Dealing with state within each request is easier (a new object is created for each request)
	 *  - It's easy to swap for a ThreadedResponder
	 *
	 * @section disadvantages_direct Disadvantages over using Manager::newRequest directly
	 *  - Added overhead of creating and destroying a QObject for each request
	 *
	 *  @see ThreadedResponder for comparison.
	 */
	class BlockingResponder : public AbstractResponder
	{
		Q_OBJECT;
		public:
			BlockingResponder(SpawnerBase* spawner, const char* responderSlot, QObject* parent = 0);
			~BlockingResponder();

			void respond(SpawnerBase* spawner, const char* responderSlot, FastCgiQt::Request*);
		public slots:
			void respond(FastCgiQt::Request*);
		private:
			class Private;
			Private* d;
	};
};
