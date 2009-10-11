#pragma once

#include <FastCgiQt/XsltService.h>

#include <QXmlStreamWriter>

class XsltExample : public FastCgiQt::XsltService
{
	Q_OBJECT;
	public:
		XsltExample(QObject* parent = 0);
		FastCgiQt::Service::UrlMap urlMap() const;
	public slots:
		void helloWorld();
	private:
		void dumpHash(const QHash<QByteArray, QByteArray>&, const QString& source, QXmlStreamWriter* xml);
};
