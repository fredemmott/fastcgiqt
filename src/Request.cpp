#include "Request.h"

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

	void Request::addServerData(const QHash<QString, QString>& data)
	{
		m_serverData.unite(data);
	}
}
