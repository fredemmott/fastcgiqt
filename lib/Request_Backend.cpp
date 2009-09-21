#include "Request_Backend.h"

#include <QStringList>

namespace FastCgiQt
{
	Request::Backend::Backend()
	: m_contentLength(0)
	, m_request(0)
	{
	}

	void Request::Backend::setRequest(Request* request)
	{
		m_request = request;
	}

	QString Request::Backend::contentType() const
	{
		return m_contentType;
	}

	quint64 Request::Backend::contentLength() const
	{
		return m_contentLength;
	}

	QByteArray Request::Backend::content() const
	{
		m_request->waitForAllContent();
		return m_content;
	}

	QHash<QString, QString> Request::Backend::serverData() const
	{
		return m_serverData;
	}

	QHash<QString, QString> Request::Backend::getData() const
	{
		return m_getData;
	}

	QHash<QString, QString> Request::Backend::postData() const
	{
		if(contentType() == QLatin1String("application/x-www-form-urlencoded"))
		{
			m_request->waitForAllContent();
		}
		return m_postData;
	}

	void Request::Backend::addServerData(const QHash<QString, QString>& data)
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

	void Request::Backend::appendContent(const QByteArray& data)
	{
		Q_ASSERT(data.length() + m_content.length() <= m_contentLength);
		m_content.append(data);
		if(static_cast<quint64>(m_content.length()) == contentLength() && contentType().startsWith("application/x-www-form-urlencoded")) // may have "; charset=FOO afterwards"
		{
			QStringList nameValuePairs = QString::fromUtf8(m_content).split(QRegExp("&|&amp;"));
			Q_FOREACH(const QString& pair, nameValuePairs)
			{
				QStringList nameValuePair = pair.split("=");
				QString name = QUrl::fromPercentEncoding(nameValuePair.first().toLatin1());
				QString value;
				if(nameValuePair.size() > 1)
				{
					value = nameValuePair.value(1);
					value.replace('+', ' ');
					value = QUrl::fromPercentEncoding(value.toLatin1());
				}
				m_postData.insert(name, value);
			}
		}
	}

};
