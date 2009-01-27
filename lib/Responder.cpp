#include "Responder.h"

#include "OutputDevice.h"

#include <QDebug>
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
			request(request),
			out(m_outputDevice)
	{
	}

	Responder::~Responder()
	{
		out << flush;
	}
}
