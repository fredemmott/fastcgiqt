#pragma once

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
