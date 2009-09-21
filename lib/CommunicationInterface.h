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
#pragma once

#include "Responder.h"

#include <QtPlugin>

#include <QObject>

namespace FastCgiQt
{
	/** Abstract interface for recieving requests from the HTTPD.
	 *
	 * This might be modified once multiple interfaces are supported,
	 * and it becomes more apparent what code can be shared.
	 *
	 * @ingroup plugins
	 */
	class CommunicationInterface : public QObject
	{
		Q_OBJECT
		public:
			virtual ~CommunicationInterface();
			virtual bool start() = 0;

			/** Factory class constructing a CommunicationInterface.
			 *
			 * This is a qt plugin interface.
			 *
			 * @ingroup plugins
			 */
			class Factory
			{
				public:
					/// Create a CommunicationInterface*
					virtual CommunicationInterface* createInterface(Responder::Generator, QObject* parent) const = 0;
			};
		protected:
			CommunicationInterface(QObject* parent);
	};
};

Q_DECLARE_INTERFACE(FastCgiQt::CommunicationInterface::Factory, "uk.co.fredemmott.FastCgiQt.CommunicationInterface/1.0");
