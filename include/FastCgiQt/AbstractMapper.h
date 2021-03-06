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
#ifndef _FASTCGIQT_ABSTRACTMAPPER
#define _FASTCGIQT_ABSTRACTMAPPER

#include "AbstractResponder.h"

namespace FastCgiQt
{
	/** Map requests to services.
	 *
	 * This class is for mapping URLs to QObjects and slots.
	 *
	 * @see AbstractMapper
	 * @ingroup services
	 */
	class AbstractMapper: public QObject
	{
		Q_OBJECT
		public:
			enum ResponseMode
			{
				BlockingResponses,
				ThreadedResponses
			};

			AbstractMapper(ResponseMode mode = BlockingResponses, QObject* parent = 0);
			virtual ~AbstractMapper();

			/// Add a spawner to the mapping.
			void addMapping(const QString& serviceName, SpawnerBase* spawner, const char* slot);
			/** Add a slot to the mapping.
			 *
			 * The receiver *MUST NOT* have a parent, and *MUST* be threadsafe.
			 */
			void addMapping(const QString& serviceName, QObject* receiver, const char* slot);
		public slots:
			void respond(FastCgiQt::Request*);
		protected:
			/** Return which of the specified service names match.
			 *
			 * @p services is a QBytearray containing the service names, in percent-encoded UTF-8.
			 */
			virtual int indexIn(const QList<QByteArray>& services) const = 0;
		private:
			class Private;
			Private* d;
	};
}
#endif // _FASTCGIQT_ABSTRACTMAPPER
