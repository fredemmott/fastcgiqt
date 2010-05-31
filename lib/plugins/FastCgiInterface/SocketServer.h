#pragma once

#include <QTcpSocket>
#include <QFlags>

namespace FastCgiQt
{
	class SocketServer : public QObject
	{
		Q_OBJECT
		public:
			enum SocketType
			{
				TcpSocket  = 1 << 0,
				UnixSocket = 1 << 1
			};
			Q_DECLARE_FLAGS(SocketTypes, SocketType);

			static SocketTypes supportedSocketTypes();
			static SocketTypes activeSocketTypes();

			SocketServer(QObject* parent = 0);
			~SocketServer();

			SocketType socketType() const;
			
			bool isFinished() const;

			bool listen(SocketType type, quint16 parameter = 0);

			/*
			 * 1. QAbstractSocket isn't abstract enough
			 * 2. QLocalSocket has entirely different semantics on Windows
			 * 3. QTcpSocket 'WorksForMe' on Linux with both socket types
			 */
			QTcpSocket* nextPendingConnection();
		signals:
			void newConnection();
		private:
			class Private;
			Private* d;
	};
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FastCgiQt::SocketServer::SocketTypes);

