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
			m_requestId(requestId)
	{
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
		m_serverData.unite(data);
	}
}
