#pragma once

#include "ClientIODevice.h"
#include "Request.h"

namespace FastCgiQt
{
	/// @internal Backend data class for Request class.
	class Request::Private
	{
		public:
			typedef QHash<QString, QString> ValueMap;

			Private(ClientIODevice* d);
			~Private();
			void loadPostVariables();
			/** The access method for the POST data of this request.
			 *
			 * As this is streamed (at least conceptually... multiplexing may prevent it
			 * in practise, eg for FastCGI), once the data's been read, it's gone.
			 *
			 * This means that each handler can either read POST data raw, via the
			 * QIODevice interface, or use the convenience methods for reading POSTed form
			 * values, but not both in the same request.
			 */
			enum PostDataMode
			{
				UnknownPostData, ///< No read of POST data has been attempted
				ValuesPostData,  ///< Read of POST form variables has been attempted
				RawPostData      ///< Read of POST data via the QIODevice has been attempted
			};

			/// The interface provided by the backend plugin.
			ClientIODevice* device;

			// Flags

			/// Whether or not we've sent headers yet.
			bool haveSentHeaders;

			/// How user-POSTed data is being used.
			PostDataMode postDataMode;

			/** The headers to send back to the client.
			 *
			 * Partially populated by constructor (defaults).
			 *
			 * @see serverData for the request headers sent by the client.
			 */
			ClientIODevice::HeaderMap responseHeaders;

			/** Variables encoded in the query string.
			 *
			 * Populated by constructor.
			 */
			ClientIODevice::HeaderMap getData;

			/** Variables sent as POST data.
			 *
			 * Populated by Request on first access.
			 *
			 * @see postDataMode
			 */
			ClientIODevice::HeaderMap postData;

			/** The CGI variables provided by the backend.
			 *
			 * Populated by constructor.
			 *
			 * Cache of device->headers().
			 */
			ClientIODevice::HeaderMap serverData;
		private:
			ClientIODevice::HeaderMap parseQueryString(const QByteArray& queryString);
	};
};
