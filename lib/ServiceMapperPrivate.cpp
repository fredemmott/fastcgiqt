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
#include "ServiceMapperPrivate.h"

#include <QDebug>

namespace FastCgiQt
{
	ServiceMapper::Private::ServiceMap ServiceMapper::Private::services;
	QMutex ServiceMapper::Private::serviceLock;

	ServiceMapper::Private::Private(ServiceMapper* parent)
	: QObject(parent)
	, m_parent(parent)
	{
		Q_ASSERT(parent);
	}

	void ServiceMapper::Private::cleanup(Service* service)
	{
		service->out << flush;
		delete service;
		m_parent->finished();
	}
};
