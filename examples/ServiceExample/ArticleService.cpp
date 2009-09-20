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
#include "ArticleService.h"

#include <QDate>
#include <QDebug>

ArticleService::ArticleService(const FastCgiQt::Request& request, QObject* parent)
	: Service(request, parent)
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
	map.append("", SLOT(list()));
	map.append("(\\d+)", SLOT(list(QString)));
	map.append("(\\d+)/(\\d+)", SLOT(list(QString, QString)));
	map.append("(\\d+)/(\\d+)/(\\d+)", SLOT(list(QString, QString,QString)));
	map.append("(\\d+)/(\\d+)/(\\d+)/(.+)", SLOT(show(QString, QString,QString, QString)));
	qDebug() << "returning" << map.count() << "methods from ArticleService";
	return map;
}
