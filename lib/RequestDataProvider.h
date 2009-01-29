#ifndef _FASTCGIQT_REQUEST_DATA_PROVIDER_H
#define _FASTCGIQT_REQUEST_DATA_PROVIDER_H

#include <QByteArray>
#include <QHash>
#include <QString>

namespace FastCgiQt
{
	class Request;
	/** @internal
	 * @brief Class for sending data to a Request object that has already
	 * been created.
	 *
	 * This is to avoid making Request::addServerData() and
	 * Request::appendContent() public.
	 */
	class RequestDataProvider
	{
		public:
			/** Add server data to the specified Request object.
			 *
			 * @see Request::addServerData()
			 */
			static void addServerData(
				Request* request,
				const QHash<QString, QString>& data
			);
			/** Append content to the specified Request object.
			 *
			 * @see Request::appendContent()
			 */
			static void appendContent(
				Request* request,
				const QByteArray& data
			);
	};
}

#endif
