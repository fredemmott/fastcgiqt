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
#ifndef _FASTCGI_QT_RESPONDER_H
#define _FASTCGI_QT_RESPONDER_H

#include "Request.h"
#include "ClientIOInterface.h"

namespace FastCgiQt
{
	/** Abstract class for responding to HTTP requests.
	 *
	 * All web applications will involve at least one subclass of this
	 * class.
	 *
	 * @ingroup core
	 */
	class Responder : public FastCgiQt::ClientIOInterface
	{
		Q_OBJECT
		public:
			/** A typedef for a pointer to a factory function for Responder objects.
			 *
			 * This function must have the signature:
			 * @code
			 * void myResponderGenerator(const Request&, QIODevice*, QIODevice*, QObject*);
			 * @endcode
			 */
			typedef Responder* (*Generator)(const Request&, QIODevice*, QIODevice* inputDevice, QObject*);

			/// Respond to a web request.
			virtual void respond() = 0;
			/** Clean up the responder.
			 *
			 * Among other things, this will flush the output stream.
			 */
			virtual ~Responder();
		protected:
			/** Construct a responder.
			 *
			 * You shouldn't actually have to do anything with these
			 * parameters except pass them through to this
			 * constructor; especially, do not use the socket
			 * directly.
			 *
			 * @param request contains information on the request.
			 * @param socket is the socket being used for FastCGI in
			 * 	this request.
			 * @param inputDevice is a streaming input device.
			 * @param parent is a QObject* parent.
			 */
			Responder(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent = NULL);
	};
}

/** Convenience macro for creating constructor and factories of Responder
 * subclasses.
 *
 * The generated constructor just calls the Responder constructor.
 *
 * The generated factory function is called 'create', and a pointer to it is
 * a FastCgiQt::Responder::Generator.
 *
 * @relates FastCgiQt::Responder
 */
#define RESPONDER(className) \
	public: \
		className(const FastCgiQt::Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent = NULL) \
			: Responder(request, socket, inputDevice, parent) {} \
		static Responder* create(const FastCgiQt::Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent) \
			{ return new className(request, socket, inputDevice, parent); } \
	private:
#endif
