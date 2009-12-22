#pragma once

#include <QAbstractSocket>
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

			SocketServer(QObject* parent = 0);
			~SocketServer();

			bool listen(SocketType type, const QString& parameter);

			bool isListening() const;

			void close();

			QAbstractSocket* nextPendingConnection();
		signals:
			void newConnection();
		private:
			class Private;
			Private* d;
	};
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FastCgiQt::SocketServer::SocketTypes);

