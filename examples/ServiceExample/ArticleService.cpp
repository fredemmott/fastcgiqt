#include "ArticleService.h"

#include <QDate>
#include <QDebug>

ArticleService::ArticleService(QObject* parent)
	: Service(parent)
{
}

void ArticleService::list()
{
	out << "List of all articles";
}

void ArticleService::list(const QString& year)
{
	out << tr("List of all articles written in %1").arg(year);
}

void ArticleService::list(const QString& year, const QString& month)
{
	out << tr("List of all articles written in %2 %1").arg(year).arg(QDate::longMonthName(month.toInt()));
}

void ArticleService::list(const QString& year, const QString& month, const QString& day)
{
	out << tr("List of all articles written on %3 %2 %1").arg(year).arg(QDate::longMonthName(month.toInt())).arg(day);
}

void ArticleService::show(const QString& year, const QString& month, const QString& day, const QString& title)
{
	out << tr("Showing article '%1'").arg(title);
}

FastCgiQt::Service::UrlMap ArticleService::urlMap() const
{
	FastCgiQt::Service::UrlMap map;
	map.insert("", SLOT(list()));
	map.insert("(\\d+)", SLOT(list(QString)));
	map.insert("(\\d+)/(\\d+)", SLOT(list(QString, QString)));
	map.insert("(\\d+)/(\\d+)/(\\d+)", SLOT(list(QString, QString,QString)));
	map.insert("(\\d+)/(\\d+)/(\\d+)/(.+)", SLOT(show(QString, QString,QString, QString)));
	qDebug() << "returning" << map.count() << "methods from ArticleService";
	return map;
}
