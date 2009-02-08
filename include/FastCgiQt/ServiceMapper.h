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
#ifndef _FASTCGI_QT_SERVICE_MAPPER_H
#define _FASTCGI_QT_SERVICE_MAPPER_H

#include "Responder.h"

namespace FastCgiQt
{
	class Service;
	/** Map requests to services.
	 *
	 * This class is for mapping URLs to QObjects and slots.
	 *
	 * @see Service
	 */
	class ServiceMapper : public Responder
	{
		Q_OBJECT
		public:
			/// Destructor.
			virtual ~ServiceMapper();
			/// Respond by calling Service::dispatchRequest on the appropriate Service.
			void respond();
		protected:
			/// Add a service to the mapping.
			void addService(const QString& serviceName, Service* service);
			/** Initialise the name -> service map.
			 *
			 * The implementation should call addService.
			 */
			virtual void loadServices() = 0;
			/// Display a message for the specified HTTP error code.
			virtual void errorMessage(int code);

			/** Constructor.
			 * @see #SERVICE_MAPPER
			 */
			ServiceMapper(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent = NULL);
		private:
			class Private;
			Private* d;
	};
}

#define SERVICE_MAPPER(className) \
	public: \
		className(const FastCgiQt::Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent = NULL) \
			: ServiceMapper(request, socket, inputDevice, parent) {} \
		static Responder* create(const FastCgiQt::Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent) \
			{ return new className(request, socket, inputDevice, parent); } \
	private:

#endif
