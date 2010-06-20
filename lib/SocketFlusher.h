#ifndef _FASTCGIQT_SOCKETFLUSHER
#define _FASTCGIQT_SOCKETFLUSHER

#include <QAbstractSocket>

namespace FastCgiQt
{
	/** Class to cleanup a socket.
	 *
	 * Takes ownership, and automatically deletes both itself and the socket
	 * when there's no more data to write to the socket.
	 */
	class SocketFlusher : public QObject
	{
		Q_OBJECT
		public:
			SocketFlusher(QAbstractSocket* socket);
		private slots:
			void deleteIfFlushed();
		private:
			QAbstractSocket* m_socket;
	};
}
#endif // _FASTCGIQT_SOCKETFLUSHER
