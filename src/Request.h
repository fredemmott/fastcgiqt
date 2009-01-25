#ifndef _FASTCGI_QT_REQUEST_H
#define _FASTCGI_QT_REQUEST_H

#include <QHash>
#include <QString>

namespace FastCgiQt
{
	class Request
	{
		public:
			Request();
			Request(quint16 requestId);
			bool isValid() const;
			quint16 requestId() const;
			QString serverData(const QString& name) const;
			QHash<QString, QString> serverData() const;
			QString getData(const QString& name) const;
			QHash<QString, QString> getData() const;

			void addServerData(const QHash<QString, QString>& data);
		private:
			bool m_isValid;
			quint16 m_requestId;
			QHash<QString, QString> m_serverData;
			QHash<QString, QString> m_getData;
	};
}

#endif
