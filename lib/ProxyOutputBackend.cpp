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
#include "ProxyOutputBackend.h"

namespace FastCgiQt
{
	ProxyOutputBackend::ProxyOutputBackend(QIODevice* source)
	: OutputDevice::Backend()
	, m_source(source)
	{
		Q_ASSERT(source->isOpen());
		Q_ASSERT(source->isWritable());
		open(QIODevice::WriteOnly | QIODevice::Unbuffered);
	}

	bool ProxyOutputBackend::waitForBytesWritten(int msecs)
	{
		return m_source->waitForBytesWritten(msecs);
	}

	qint64 ProxyOutputBackend::writeData(const char* data, qint64 maxSize)
	{
		return m_source->write(data, maxSize);
	}
};
