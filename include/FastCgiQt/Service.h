#ifndef _FASTCGI_QT_SERVICE_H
#define _FASTCGI_QT_SERVICE_H

#include "ClientIOInterface.h"

#include <QMap>
#include <QObject>
#include <QRegExp>
#include <QString>

namespace FastCgiQt
{
	class ServicePrivate;
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
	 * @see ServiceMapper
	 */
	class Service : public ClientIOInterface
	{
		friend class ServiceMapper;
		friend class ServicePrivate;
		Q_OBJECT
		public:
			/** @internal
			 * @brief Call a slot based on an url fragment.
			 *
			 * The mapping depends on what urlMap() returns.
			 *
			 * @see urlMap()
			 */
			void dispatchRequest(const QString& urlFragment);

			/// Destructor.
			virtual ~Service();
		protected:
			/** A map of regular expressions to slots.
			 *
			 * This is a pair of regularExpressionString ->
			 * publicSlot - for example:
			 * 	myUrlMap.insert("(.+)", SLOT(foo(QString)));
			 *
			 * @see urlMap()
			 */
			typedef QMap<QString, const char*> UrlMap;

			/** Return a mapping of regular expressions to slots.
			 *
			 * This is used to map requests.
			 *
			 * @see dispatchRequest()
			 * @see UrlMap
			 */
			virtual UrlMap urlMap() const = 0;
			/// Constructor.
			Service(QObject* parent = NULL);
		private:
			ServicePrivate* d;
	};
}

#endif
