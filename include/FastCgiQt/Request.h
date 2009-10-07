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

#pragma once

#include <QByteArray>
#include <QIODevice>
#include <QList>
#include <QUrl>

class QNetworkCookie;

namespace FastCgiQt
{
	enum UrlPart
	{
		RootUrl,     ///< Root URL of the application.
		LocationUrl, ///< The page requested, without the query string.
		RequestUrl   ///< The full request URL, including query string.
	};

	enum DataSource
	{
		GetData,   ///< ?foo=bar&baz=meh on query string
		PostData,  ///< Only valid if CONTENT_TYPE is application/x-www-formencoded
		ServerData ///< CGI-compatible environment variable names
	};

	/** Object representing a request;
	 * @ingroup core
	 */
	class Request : public QIODevice
	{
		Q_OBJECT;
		public:
			virtual ~Request();

			void setHeader(const QByteArray& name, const QByteArray& value);
			void addHeader(const QByteArray& name, const QByteArray& value);
			QList<QNetworkCookie> cookies() const;
			void sendCookie(const QNetworkCookie&);

			QHash<QByteArray, QByteArray> rawValues(DataSource) const;
			QByteArray rawValue(DataSource, const QByteArray& name) const;
			QString value(DataSource, const QByteArray& name) const;

			/** Various parts of the URL for this request.
			 *
			 * If you just want the path relative to the root of the application, that's
			 * value(ServerData, "PATH_INFO");
			 */
			QUrl url(UrlPart) const;

			// QIODevice overrides start here

			/** Amount of POST data.
			 *
			 * Convenience wrapper around value(ServerData, "CONTENT_LENGTH"))
			 */
			virtual qint64 size() const;
		signals:
			/** Hook to do any cleanup.
			 *
			 * If you're doing any threading yourself and need the object to still be valid,
			 * make sure you use either Qt::DirectConnection or Qt::BlockingQueuedConnection,
			 * otherwise the object might not exist when your slot gets called.
			 */
			void finished(FastCgiQt::Request* request);
		protected:
			virtual qint64 readData(char* data, qint64 maxSize);
			virtual qint64 writeData(const char* data, qint64 maxSize);
		private:
			friend class RequestFactory;
			class Private;
			Private* d;
			Request(Private*, QObject* parent);
	};
}
