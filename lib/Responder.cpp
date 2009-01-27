#include "Responder.h"

#include "OutputDevice.h"

#include <QDebug>
#include <QIODevice>

namespace FastCgiQt
{
	Responder::Responder(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent)
		:
			QObject(parent),
			request(request),
			in(inputDevice),
			out(
				new OutputDevice(
					request.requestId(),
					socket,
					this
				)
			)
	{
	}

	Responder::~Responder()
	{
		out << flush;
	}
}
