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
#include "Base64Decoder_Private.h"

#include <QDebug>

namespace FastCgiQt
{
	Base64Decoder::Private::Private(QIODevice* source)
	: m_source(source)
	{
	}

	Base64Decoder::Private::~Private()
	{
	}

	QByteArray Base64Decoder::Private::readWithoutWhitespace(int count)
	{
		QByteArray out;
		while(out.length() < count && !m_source->atEnd())
		{
			char foo;
			if(!m_source->getChar(&foo))
			{
				break;
			}
			if(!QChar(foo).isSpace())
			{
				out.append(foo);
			}
		}
		return out;
	}
};
