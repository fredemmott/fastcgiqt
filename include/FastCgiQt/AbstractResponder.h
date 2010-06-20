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
#ifndef _FASTCGIQT_ABSTRACTRESPONDER
#define _FASTCGIQT_ABSTRACTRESPONDER

#include <QObject>

namespace FastCgiQt
{
	class Request;

	/** Interface for a QObject factory class.
	 *
	 * @see Spawner
	 */
	class SpawnerBase {
		public:
			SpawnerBase();
			virtual QObject* spawn(QObject* parent) const = 0;
			virtual bool persist() const = 0;
			virtual ~SpawnerBase();
	};

	/** Template for a QObject factory class.
	 *
	 * @see SpawnerBase
	 * @see ThreadedResponder
	 */
	template<class T> class Spawner : public SpawnerBase
	{
		public:
			QObject* spawn(QObject* parent) const
			{
				return new T(parent);
			}
			bool persist() const
			{
				return false;
			}
	};

	/** Interface for responder classes, spawning instances of your object.
	 *
	 * @see BlockingResponder for a simple implementation, and @see ThreadedResponder
	 * for a multi-threaded implementation.
	 */
	class AbstractResponder : public QObject
	{
		Q_OBJECT;
		public:
			AbstractResponder(QObject* parent);
			virtual ~AbstractResponder();

			virtual void respond(SpawnerBase* spawner, const char* responderSlot, FastCgiQt::Request*) = 0;
		public slots:
			virtual void respond(FastCgiQt::Request*) = 0;
	};
};
#endif // _FASTCGIQT_ABSTRACTRESPONDER
