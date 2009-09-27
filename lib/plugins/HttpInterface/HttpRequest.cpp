#include "HttpRequest.h"

#include <QBuffer>
#include <QFileInfo>
#include <QHostInfo>
#include <QTcpSocket>

namespace FastCgiQt
{
	HttpRequest::HttpRequest(const HeaderMap& standardRequestHeaders, const HeaderMap& standardResponseHeaders, const QStringList& staticDirectories, QTcpSocket* socket, QObject* parent)
	: ClientIODevice(parent)
	, m_requestState(WaitingForRequest)
	, m_responseState(WaitingForResponseHeaders)
	, m_requestHeaders(standardRequestHeaders)
	, m_responseHeaders(standardResponseHeaders)
	, m_socket(socket)
	, m_staticDirectories(staticDirectories)
	{
		const QHostInfo serverInfo = QHostInfo::fromName(socket->localAddress().toString());
		m_requestHeaders.insert("SERVER_NAME", serverInfo.hostName().toLatin1());
		m_requestHeaders.insert("SERVER_ADDR", socket->localAddress().toString().toLatin1());
		m_requestHeaders.insert("REMOTE_PORT", QString::number(socket->peerPort()).toLatin1());
		m_requestHeaders.insert("REMOTE_ADDR", socket->peerAddress().toString().toLatin1());
		open(QIODevice::ReadWrite | QIODevice::Unbuffered);
		socket->setParent(this);
		Q_ASSERT(socket->isOpen());
		Q_ASSERT(socket->isWritable());
		Q_ASSERT(socket->isReadable());
		connect(
			socket,
			SIGNAL(readyRead()),
			this,
			SLOT(readSocketData())
		);
	}

	HttpRequest::~HttpRequest()
	{
		m_socket->flush();
		m_socket->close();
	}

	ClientIODevice::HeaderMap HttpRequest::requestHeaders() const
	{
		return m_requestHeaders;
	}

	void HttpRequest::readSocketData()
	{
		if(m_requestState == WaitingForRequest)
		{
			if(!m_socket->canReadLine())
			{
				return;
			}
			const QByteArray line = m_socket->readLine().trimmed();
			const QList<QByteArray> parts = line.split(' ');
			Q_ASSERT(parts.count() == 3);
			m_requestHeaders.insert("REQUEST_METHOD", parts.at(0));
			m_requestHeaders.insert("SERVER_PROTOCOL", parts.at(2));
			const QByteArray uri = parts.at(1);
			m_requestHeaders.insert("REQUEST_URI", uri);
			const int startOfQueryString = uri.indexOf('?') + 1;
			m_requestHeaders.insert("PATH_INFO", uri.mid(0, startOfQueryString - 1));
			if(startOfQueryString > 0)
			{
				m_requestHeaders.insert("QUERY_STRING", uri.mid(startOfQueryString));
			}
			m_requestState = WaitingForRequestHeaders;
			readSocketData();
			return;
		}
		if(m_requestState == WaitingForRequestHeaders)
		{
			while(m_socket->canReadLine())
			{
				const QByteArray line = m_socket->readLine().trimmed();
				if(line.isEmpty())
				{
					m_requestState = WaitingForRequestBody;
					dispatchRequest();
					readSocketData();
					return;
				}
				const int lengthOfName = line.indexOf(':');
				QByteArray name = line.left(lengthOfName).toUpper();
				name.replace('-', '_');
				const QByteArray value = line.mid(lengthOfName + 2); // ": " after the name == 2 chars
				if(name == "CONTENT_LENGTH" || name == "CONTENT_TYPE")
				{
					m_requestHeaders.insert(name, value);
				}
				else
				{
					m_requestHeaders.insert("HTTP_" + name, value);
				}
			}
			return;
		}
		emit readyRead();
	}

	void HttpRequest::dispatchRequest()
	{
		if(!m_staticDirectories.isEmpty())
		{
			const QString fullFilePath = QFileInfo("./" + m_requestHeaders.value("REQUEST_URI")).canonicalFilePath();
			Q_FOREACH(const QString& path, m_staticDirectories)
			{
				if(fullFilePath.startsWith(path))
				{
					QFile file(fullFilePath);
					file.open(QIODevice::ReadOnly);
					if(file.isOpen())
					{
						QTextStream cout(stdout);
						cout << "***** Sending file '" << fullFilePath << "' as static content." << endl;
						m_responseHeaders.insert("STATUS", "200 OK");
						m_responseHeaders.insert("CONTENT-LENGTH", QString::number(file.size()).toLatin1());
						write("\r\n");
						write(file.readAll());
						deleteLater();
						return;
					}
					break;
				}
			}
		}
		// Pass off to FastCgiQt
		emit gotHeaders(this);
	}

	qint64 HttpRequest::readData(char* data, qint64 maxSize)
	{
		Q_ASSERT(m_requestState == WaitingForRequestBody);
		return m_socket->read(data, maxSize);
	}

	qint64 HttpRequest::writeData(const char* data, qint64 maxSize)
	{
		if(m_responseState == WaitingForResponseHeaders)
		{
			// We need to buffer the headers, so we can use the STATUS header appropriately
			QBuffer buffer;
			buffer.setData(data, maxSize);
			buffer.open(QIODevice::ReadOnly);
			while(buffer.canReadLine())
			{
				const QByteArray line = buffer.readLine().trimmed();
				if(line.isEmpty())
				{
					Q_ASSERT(m_responseHeaders.contains("STATUS"));
					Q_ASSERT(m_requestHeaders.contains("SERVER_PROTOCOL"));
					m_responseState = WaitingForResponseBody;
					const QByteArray status = m_responseHeaders.take("STATUS");
					m_socket->write(m_requestHeaders.value("SERVER_PROTOCOL"));
					m_socket->write(" ", 1);
					m_socket->write(status);
					m_socket->write("\r\n", 2);

					//qDebug() << Q_FUNC_INFO << m_requestHeaders << m_responseHeaders;

					for(
						HeaderMap::ConstIterator it = m_responseHeaders.constBegin();
						it != m_responseHeaders.constEnd();
						++it
					)
					{
						m_socket->write(it.key());
						m_socket->write(": ");
						m_socket->write(it.value());
						m_socket->write("\r\n");
					}
					m_socket->write("\r\n");
					m_socket->write(buffer.readAll());
					return maxSize;
				}
				const int lengthOfName = line.indexOf(':');
				const QByteArray name = line.left(lengthOfName);
				const QByteArray value = line.mid(lengthOfName + 2); // ": " after the name == 2 chars
				m_responseHeaders.insert(name, value);
			}
			return maxSize;
		}
		Q_ASSERT(m_responseState == WaitingForResponseBody);
		return m_socket->write(data, maxSize);
	}
};
