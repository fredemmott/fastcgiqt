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
#ifndef _FASTCGIQT_MIMEMULTIPARTSTREAMREADER
#define _FASTCGIQT_MIMEMULTIPARTSTREAMREADER

class QByteArray;
class QIODevice;

#include <QString>

namespace FastCgiQt
{
	class MimeMultipartStreamReader
	{
		public:
			enum TokenType
			{
				Invalid        = 0,
				NoToken        = 1,
				StartDocument  = 2, 
				DocumentHeader = 3,
				IgnoredContent = 4,
				EndPart        = 5,
				StartPart      = 6,
				PartHeader     = 7,
				PartContent    = 8,
				EndDocument    = 9
			};
			/// Create a new MimeMultipartStreamReader, expecting document headers.
			MimeMultipartStreamReader(QIODevice* device);
			/// Create a new MimeMultipartStreamReader, expecting the first (ignored) content blob
			MimeMultipartStreamReader(QIODevice* device, const QByteArray& boundary);
			~MimeMultipartStreamReader();
			bool atEnd() const;
			QIODevice* device() const;
			TokenType tokenType() const;
			TokenType readNext();

			/** If tokenType() is DocumentHeader or PartHeader, returns the header name.
			 *
			 * Otherwise, an empty string is returned.
			 */
			QString headerName() const;
			/** If tokenType() is DocumentHeader or PartHeader, returns the header value.
			 *
			 * Otherwise, an empty string is returned.
			 */
			QString headerValue() const;

			/** If tokenType() is IgnoredContent or PartContent, returns a QIODevice for reading the part.
			 *
			 * Otherwise, a null pointer is returned.
			 *
			 * The QIODevice remains owned by this object, and must not be deleted by the caller.
			 */
			QIODevice* content() const;
		private:
			class Private;
			Private* d;
	};
};
#endif // _FASTCGIQT_MIMEMULTIPARTSTREAMREADER
