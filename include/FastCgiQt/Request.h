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
#ifndef _FASTCGI_QT_REQUEST_H
#define _FASTCGI_QT_REQUEST_H

#include <QHash>
#include <QString>
#include <QUrl>

class QNetworkCookie;

namespace FastCgiQt
{
	/** Class containing information about a request.
	 * @ingroup core
	 */
	class Request
	{
		friend class RequestDataProvider;
		public:
			/// Construct an invalid Request object.
			Request();
			/** Construct a valid Request object, for the specified
			 * request id.
			 *
			 * @param requestId is a FastCGI request ID.
			 */
			Request(quint16 requestId);

			/** If this Request object is actually valid.
			 *
			 * An invalid Request object is not actually associated
			 * with a client request.
			 */
			bool isValid() const;
			/// The FastCGI request ID.
			quint16 requestId() const;

			/** The type of the POST-data (if any).
			 *
			 * @see contentLength()
			 * @see content()
			 * @see haveAllContent()
			 * @see waitForAllContent()
			 * @see postData(const QString&) const
			 * @see postData() const
			 */
			QString contentType() const;

			/** The length of the POST data.
			 *
			 * @see contentType()
			 * @see content()
			 * @see haveAllContent()
			 * @see waitForAllContent()
			 * @see postData(const QString&) const
			 * @see postData() const
			 */
			quint64 contentLength() const;

			/** The POST data sent in this request.
			 *
			 * If it has not all been received, this method will
			 * block until the upload has been finished
			 *
			 *
			 * @see Responder::in for a stream.
			 * @see contentType()
			 * @see contentLength()
			 * @see haveAllContent()
			 * @see waitForAllContent()
			 * @see postData(const QString&) const
			 * @see postData() const
			 */
			QByteArray content() const;

			/** Whether all the POST data has been uploaded by the
			 * client.
			 *
			 * @see contentType()
			 * @see contentLength()
			 * @see content()
			 * @see waitForAllContent()
			 * @see postData(const QString&) const
			 * @see postData() const
			 */
			bool haveAllContent() const;

			/** Block until all POST data has been uploaded by the
			 * client.
			 *
			 * @see contentType()
			 * @see contentLength()
			 * @see content()
			 * @see haveAllContent()
			 * @see postData(const QString&) const
			 * @see postData() const
			 */
			void waitForAllContent() const;

			/** Retrieve a POST form value.
			 *
			 * If the content-type is not
			 * application/x-www-urlencoded, or the specified form
			 * value wasn't sent, this will return a null QString.
			 *
			 * If the content-type is application/x-www-urlencoded,
			 * but there is still POST data which hasn't been
			 * received yet, this member will block until it has all
			 * been received.
			 *
			 * This is roughly equivalent to $_POST[$name] in PHP.
			 *
			 * @param name is the name of the value to try and
			 * 	retrieve.
			 *
			 * @see contentType()
			 * @see contentLength()
			 * @see content()
			 * @see haveAllContent()
			 * @see waitForAllContent()
			 * @see postData() const
			 */
			QString postData(const QString& name) const;

			/** Retrieve all POST form data.
			 *
			 * This will return an empty QHash if the content-type
			 * is not application/x-www-urlencoded, or if there
			 * were no properly-encoded name-value pairs.
			 *
			 * If the content-type is application/x-www-urlencoded,
			 * but there is still POST data which hasn't been
			 * received yet, this member will block until it has all
			 * been received.
			 *
			 * This is roughly equivalent to $_POST in PHP.
			 *
			 * @see contentType()
			 * @see contentLength()
			 * @see content()
			 * @see haveAllContent()
			 * @see waitForAllContent()
			 * @see postData(const QString&) const
			 */
			QHash<QString, QString> postData() const;

			/** Retrieve a specific server variable.
			 *
			 * If the specified server variable is not set, this
			 * will return a null QString.
			 *
			 * This is roughly equivalent to $_SERVER[$name] in PHP.
			 *
			 * @param name is the name of the server variable to be
			 * 	retrieved.
			 *
			 * @see serverData() const
			 */
			QString serverData(const QString& name) const;

			/** Retrieve all server variables.
			 *
			 * These variables are equivalent to the environment
			 * variables used by CGI, such as the HTTP headers,
			 * QUERY_STRING, and so on.
			 *
			 * This is roughly equivalent to $_SERVER in PHP.
			 *
			 * @see serverData(const QString&) const
			 */
			QHash<QString, QString> serverData() const;

			/** Retrieve a GET form value.
			 *
			 * This returns a value which is form-encoded in the
			 * query string.
			 *
			 * This is roughly equivalent to $_GET[$name] in PHP.
			 *
			 * @param name is the name of the value to retrieve.
			 *
			 * @see getData() const
			 */
			QString getData(const QString& name) const;

			/** Retrieve all GET form values.
			 *
			 * This returns all name-value form data pairs from the
			 * query string.
			 *
			 * This is roughly equivalent to $_GET in PHP.
			 */
			QHash<QString, QString> getData() const;

			/** Return a guess at the base URI for this script.
			 *
			 * This is useful for creating relative links.
			 */
			QString baseUri() const;

			/// Full path, including query string.
			QString fullUri() const;

			/// Full URL
			QUrl url() const;

			/// A list of all the cookies provided by the client
			QList<QNetworkCookie> cookies() const;
		private:
			/** Add some newly-received server variables to this
			 * Request object.
			 *
			 * This should be called by RequestDataProvider.
			 */
			void addServerData(const QHash<QString, QString>& data);

			/** Add some newly-received post data.
			 *
			 * This should be called by RequestDataProvider.
			 */
			void appendContent(const QByteArray& data);

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
