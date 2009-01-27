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

			QString contentType() const;
			quint64 contentLength() const;
			QByteArray content() const;
			bool haveAllContent() const;
			void waitForAllContent() const;

			QString postData(const QString& name) const;
			QHash<QString, QString> postData() const;

			QString serverData(const QString& name) const;
			QHash<QString, QString> serverData() const;

			QString getData(const QString& name) const;
			QHash<QString, QString> getData() const;

			void addServerData(const QHash<QString, QString>& data);
			void appendContent(const QByteArray& data);
		private:
			bool m_isValid;
			quint16 m_requestId;
			QHash<QString, QString> m_serverData;
			QHash<QString, QString> m_getData;
			QHash<QString, QString> m_postData;

			QString m_contentType;
			quint16 m_contentLength;
			QByteArray m_content;
	};
}

#endif
