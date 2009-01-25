#ifndef _FASTCGI_QT_PARAMETERS_RECORD_H
#define _FASTCGI_QT_PARAMETERS_RECORD_H

#include <QByteArray>
#include <QHash>
#include <QString>

namespace FastCgiQt
{
	class RecordHeader;
	class ParametersRecord
	{
		public:
			ParametersRecord(const RecordHeader& header, const QByteArray& data);
			quint16 requestId() const;
			QHash<QString, QString> parameters() const;
		private:
			quint16 m_requestId;
			QHash<QString, QString> m_parameters;
	};
}

#endif
