#include "Responder.h"

#include "OutputDevice.h"

#include <QIODevice>

namespace FastCgiQt
{
	Responder::Responder(const Request& request, QIODevice* socket, QObject* parent)
		:
			QObject(parent),
			m_outputDevice(
				new OutputDevice(
					request.requestId(),
					socket,
					this
				)
			),
			m_outputStream(m_outputDevice),
			m_request(request)
	{
	}

	Responder::~Responder()
	{
	}

	const Request& Responder::request() const
	{
		return m_request;
	}

	QTextStream& Responder::out()
	{
		return m_outputStream;
	}
}
