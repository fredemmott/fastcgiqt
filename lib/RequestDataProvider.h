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
