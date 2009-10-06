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
#include "Manager.h"

#include "Manager_Private.h"
#include "Settings.h"

namespace FastCgiQt
{
	Manager::Manager(QObject* parent)
		:
			QObject(parent),
			d(new Private(this))
	{
		connect(
			d,
			SIGNAL(newRequest(FastCgiQt::Request*)),
			this,
			SIGNAL(newRequest(FastCgiQt::Request*))
		);
	}

	Manager::Manager(const char* backend, const QMap<QString, QVariant>& configuration, QObject* parent)
		:
			QObject(parent),
			d(0)
	{
		// todo: some nicer way
		{
			Settings settings;
			settings.beginGroup("FastCGI");
			settings.setValue("socketType", QString::fromLatin1(backend));
			for(
				QMap<QString, QVariant>::ConstIterator it = configuration.constBegin();
				it != configuration.constEnd();
				++it
			)
			{
				settings.setValue(it.key(), it.value());
			}
			settings.sync();
		}

		d = new Private(this);
		connect(
			d,
			SIGNAL(newRequest(FastCgiQt::Request*)),
			this,
			SIGNAL(newRequest(FastCgiQt::Request*))
		);
	}

	Manager::~Manager()
	{
	}
}
