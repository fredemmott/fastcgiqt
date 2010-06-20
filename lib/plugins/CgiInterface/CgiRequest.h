/* LICENSE NOTICE
	Copyright (c) 2010, Frederick Emmott <mail@fredemmott.co.uk>

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

#ifndef _FASTCGIQT_CGIREQUEST
#define _FASTCGIQT_CGIREQUEST

#include "ClientIODevice.h"

namespace FastCgiQt
{
	class CgiRequest : public ClientIODevice
	{
		Q_OBJECT;
		public:
			CgiRequest(QObject* parent);
			~CgiRequest();
			HeaderMap requestHeaders() const;
		protected:
			qint64 readData(char* data, qint64 maxSize);
			qint64 writeData(const char* data, qint64 maxSize);
		private:
			HeaderMap m_requestHeaders;
	};
}
#endif // _FASTCGIQT_CGIREQUEST
