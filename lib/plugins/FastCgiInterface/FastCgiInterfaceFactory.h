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
#ifndef _FASTCGIQT_FASTCGIINTERFACEFACTORY
#define _FASTCGIQT_FASTCGIINTERFACEFACTORY

#include "CommunicationInterface.h"

namespace FastCgiQt
{
	class FastCgiInterfaceFactory: public QObject, public CommunicationInterface::Factory
	{
		Q_OBJECT
		Q_INTERFACES(FastCgiQt::CommunicationInterface::Factory)
		public:
			virtual CommunicationInterface* createInterface(QObject* parent) const;
	};
}
#endif // _FASTCGIQT_FASTCGIINTERFACEFACTORY
