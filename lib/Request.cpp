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
#include "Request.h"

#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QUrl>

namespace FastCgiQt
{
	Request::Request()
		:
			m_isValid(false)
	{
	}

	Request::Request(quint16 requestId)
		:
			m_isValid(true),
			m_requestId(requestId),
			m_contentLength(0)
	{
	}

	QString Request::contentType() const
	{
		return m_contentType;
	}

	quint64 Request::contentLength() const
	{
		return m_contentLength;
	}

	void Request::waitForAllContent() const
	{
		while(!haveAllContent())
		{
			QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
		}
	}

	QByteArray Request::content() const
	{
		waitForAllContent();
		return m_content;
	}

	bool Request::haveAllContent() const
	{
		qDebug() << "Expecting" << contentLength() << "and got" << m_content.length();
		return contentLength() == static_cast<quint64>(m_content.length());
	}

	bool Request::isValid() const
	{
		return m_isValid;
	}

	quint16 Request::requestId() const
	{
		return m_requestId;
	}

	QString Request::serverData(const QString& name) const
	{
		return m_serverData.value(name);
	}

	QHash<QString, QString> Request::serverData() const
	{
		return m_serverData;
	}

	QString Request::getData(const QString& name) const
	{
		return m_getData.value(name);
	}

	QHash<QString, QString> Request::getData() const
	{
		return m_getData;
	}

	void Request::addServerData(const QHash<QString, QString>& data)
	{
		if(data.contains("QUERY_STRING"))
		{
			QStringList nameValuePairs = data.value("QUERY_STRING").split("&");
			Q_FOREACH(const QString& pair, nameValuePairs)
			{
				QStringList nameValuePair = pair.split("=");
				QString name = QUrl::fromPercentEncoding(nameValuePair.first().toLatin1());
				QString value;
				if(nameValuePair.size() > 1)
				{
					value = QUrl::fromPercentEncoding(nameValuePair.value(1).toLatin1());
				}
				m_getData.insert(name, value);
			}
		}
		if(data.contains("CONTENT_TYPE"))
		{
			m_contentType = data.value("CONTENT_TYPE");
		}
		if(data.contains("CONTENT_LENGTH"))
		{
			m_contentLength = data.value("CONTENT_LENGTH").toULongLong();
		}

		m_serverData.unite(data);
	}

	void Request::appendContent(const QByteArray& data)
	{
		Q_ASSERT(data.length() + m_content.length() <= m_contentLength);
		m_content.append(data);
		if(static_cast<quint64>(m_content.length()) == contentLength() && contentType() == "application/x-www-form-urlencoded")
		{
			QStringList nameValuePairs = QString::fromUtf8(m_content).split("&");
			Q_FOREACH(const QString& pair, nameValuePairs)
			{
				QStringList nameValuePair = pair.split("=");
				QString name = QUrl::fromPercentEncoding(nameValuePair.first().toLatin1());
				QString value;
				if(nameValuePair.size() > 1)
				{
					value = QUrl::fromPercentEncoding(nameValuePair.value(1).toLatin1());
				}
				m_postData.insert(name, value);
			}
		}
	}

	QString Request::postData(const QString& name) const
	{
		if(contentType() == QLatin1String("application/x-www-form-urlencoded"))
		{
			waitForAllContent();
		}
		return m_postData.value(name);
	}

	QHash<QString, QString> Request::postData() const
	{
		if(contentType() == QLatin1String("application/x-www-form-urlencoded"))
		{
			waitForAllContent();
		}
		waitForAllContent();
		return m_postData;
	}
}
