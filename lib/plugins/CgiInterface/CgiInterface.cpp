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

#include "CgiInterface.h"
#include "CgiRequest.h"

#include <QDebug>
#include <QStringList>
#include <QTimer>

#include <cstdlib>

namespace FastCgiQt
{
	CgiInterface::CgiInterface(QObject* parent)
	: CommunicationInterface(parent)
	{
	}

	CgiInterface::~CgiInterface()
	{
	}

	bool CgiInterface::isFinished() const
	{
		return true;
	}

	QStringList CgiInterface::backends() const
	{
		return QStringList() << "CGI";
	}

	QStringList CgiInterface::detectedBackends() const
	{
		if(QLatin1String(getenv("GATEWAY_INTERFACE")) == "CGI/1.1")
		{
			return QStringList() << "CGI";
		}
		return QStringList();
	}

	void CgiInterface::init()
	{
		addRequest(new CgiRequest(this));
	}

	bool CgiInterface::startBackend(const QString& backend)
	{
		Q_ASSERT(backend == "CGI");
		Q_UNUSED(backend);
		QTimer::singleShot(0, this, SLOT(init()));
		return true;
	}
}
