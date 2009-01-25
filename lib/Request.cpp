#include "Request.h"

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

	QByteArray Request::content() const
	{
		// If this fails, still waiting for some STDIN records
		Q_ASSERT(m_content.length() == m_contentLength);
		return m_content;
	}

	bool Request::haveContent() const
	{
		return contentLength() == m_content.length();
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
	}
}
