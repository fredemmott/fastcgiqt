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
#ifndef _FASTCGI_QT_SERVICE_H
#define _FASTCGI_QT_SERVICE_H

#include "ClientIOInterface.h"
#include "Request.h"

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QRegExp>
#include <QString>

namespace FastCgiQt
{
	/** Interface exposing public slots to the web.
	 *
	 * urlMap() should be overridden to provide a regex->slot mapping;
	 * dispatchRequest() will call the first slot in this map with a
	 * matching regex.
	 *
	 * QString parameters to the slots are supported, as long as the number
	 * of parameters is the same as the number of captured subexpressions in
	 * expression in the map.
	 *
	 * Service subclasses should be used in conjunction with a ServiceMapper
	 * subclass, providing an easy way to map /foo/bar/baz to Foo::bar(baz)
	 * (for example).
	 *
	 * These objects are semi-persistent, in that one will be created per
	 * thread (which may be more than one request) - there is no guarantee
	 * that all requests in the same session will go to the same instance of
	 * your subclass.
	 *
	 * Service also maintains two caches:
	 *  - A file cache
	 *  - A request cache
	 *
	 * The file cache caches data from the readFile call.
	 *
	 * The request cache caches urlFragment -> result pairs. This is only
	 * used if a slot calls canCacheThisRequest(). You can expire a cache
	 * entry by reimplementing isExpired.
	 *
	 * These caches are currently per-process. In the future, they may be
	 * shared between processes via memcached or similar. They default to a
	 * maximum size of 10MiB.
	 *
	 * @see ServiceMapper
	 */
	class Service : public ClientIOInterface
	{
		friend class ServiceMapper;
		friend class ServicePrivate;
		Q_OBJECT
		public:
			/** A typedef for a pointer to a factory function for Service objects.
			 *
			 * This function must have the signature:
			 * @code
			 * void myServiceGenerator(QObject*);
			 * @endcode
			 */
			typedef Service* (*Generator)(const Request& request, QObject* parent);
			/** @internal
			 * @brief Call a slot based on an url fragment.
			 *
			 * The result may be cached.
			 *
			 * The mapping depends on what urlMap() returns.
			 *
			 * If you want to override this function, see
			 * dispatchUncachedRequest()
			 *
			 * @see dispatchUncachedRequest()
			 * @see urlMap()
			 */
			void dispatchRequest(const QString& urlFragment);

			/// Destructor.
			virtual ~Service();
		protected:
			/** @internal
			 * @brief Call a slot based on an URL fragment.
			 *
			 * This should not maintain an urlFragment -> result map,
			 * as this is done by Service.
			 *
			 * @see dispatchRequest()
			 */
			virtual void dispatchUncachedRequest(const QString& urlFragment);
			/** A map of regular expressions to slots.
			 *
			 * This is a pair of regularExpressionString ->
			 * publicSlot - for example:
			 * 	myUrlMap.insert("(.+)", SLOT(foo(QString)));
			 *
			 * @see urlMap()
			 */
			class UrlMap : public QList<QPair<QString, const char*> >
			{
				public:
					void append(const QString& regexp, const char* slot);
			};

			/** The size of the request cache.
			 *
			 * @see canCacheThisRequest()
			 * @see isExpired()
			 * @see setRequestCacheSize()
			 */
			static int requestCacheSize();

			/** Set the size of the request cache.
			 *
			 * @see canCacheThisRequest()
			 * @see isExpired()
			 * @see requestCacheSize()
			 */
			static void setRequestCacheSize(int newSize);

			/** Next time a similar request is made, return the same data.
			 *
			 * To have effect, this must be called before any data
			 * is written to the output device.
			 *
			 * @see isExpired()
			 * @see requestCacheSize()
			 * @see setRequestCacheSize()
			 */
			void canCacheThisRequest();

			/** Return a mapping of regular expressions to slots.
			 *
			 * This is used to map requests.
			 *
			 * @see dispatchRequest()
			 * @see UrlMap
			 */
			virtual UrlMap urlMap() const = 0;

			/** Whether or not file reads are cached.
			 *
			 * If this is true, all file reads via readFile will be
			 * cached, if the file size is less than the maximum
			 * cache size.
			 *
			 * @see setUsingFileCache
			 * @see clearFileCache
			 * @see fileCacheSize
			 * @see setFileCacheSize
			 * @see readFile
			 */
			static bool usingFileCache();

			/** Set whether or not file reads via readFile() are cached.
			 *
			 * @see usingFileCache
			 * @see clearFileCache
			 * @see fileCacheSize
			 * @see setFileCacheSize
			 * @see readFile
			 */
			static void setUsingFileCache(bool use);

			/** Clear the file cache used by readFile().
			 *
			 * @see usingFileCache
			 * @see setUsingFileCache
			 * @see fileCacheSize
			 * @see setFileCacheSize
			 * @see readFile
			 */
			static void clearFileCache();

			/** The maximum size of the cache for readFile().
			 *
			 * @see usingFileCache
			 * @see clearFileCache
			 * @see setUsingFileCache
			 * @see setFileCacheSize
			 * @see readFile
			 */
			static int fileCacheSize();

			/** Set the maximum size of the cache for readFile().
			 *
			 * @see usingFileCache
			 * @see clearFileCache
			 * @see setUsingFileCache
			 * @see fileCacheSize
			 * @see readFile
			 */
			static void setFileCacheSize(int maximumSize);

			/** Read a file, optionally using a per-process file cache.
			 *
			 * If @p useCache is true, the cache is enabled, and the
			 * file is smaller than the maximum size of the cache,
			 * the file cache will be used.
			 *
			 * If this would put the cache size over the maximum,
			 * the least-recently-accessed file will be removed from
			 * the cache.
			 *
			 * A cache entry will automatically expire when the file
			 * is modified.
			 *
			 * @see usingFileCache
			 * @see clearFileCache
			 * @see setUsingFileCache
			 * @see fileCacheSize
			 * @see setFileCacheSize
			 */
			QByteArray readFile(const QString& path, bool useCache = true);

			/** Whether or not a cached result for the given urlFragment is still valid.
			 *
			 * @param urlFragment is the URL fragment for the request.
			 * @param generated is the time the cached version was generated.
			 *
			 * If you have content that may change, but you wish to
			 * cache the results, you'll want to reimplement this
			 * function, and make your slots call canCacheThisRequest().
			 *
			 * If the cached result is invalid, you should return
			 * true, and refresh any internal caches in your service.
			 *
			 * @see canCacheThisRequest()
			 * @see requestCacheSize()
			 * @see setRequestCacheSize()
			 * @returns false
			 */
			virtual bool isExpired(const QString& urlFragment, const QDateTime& generated);
			/// Constructor.
			Service(const Request& request, QObject* parent = NULL);
		private:
			class Private;
			Private* d;
	};
}

/** Convenience macro for creating factories of Service subclasses.
 *
 * The generated constructor just calls the Responder constructor.
 *
 * The generated factory function is called 'create', and a pointer to it is
 * a FastCgiQt::Service::Generator.
 *
 * @relates FastCgiQt::Service
 */
#define SERVICE(className) \
	public: \
		static Service* create(const FastCgiQt::Request& request, QObject* parent) { return new className(request, parent); } \
	private:

#endif
