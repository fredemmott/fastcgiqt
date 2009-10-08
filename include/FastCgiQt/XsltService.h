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
#ifndef _XSLT_SERVICE_H
#define _XSLT_SERVICE_H

#include "Service.h"

#include <QMap>
#include <QUrl>
#include <QVariant>
#include <QXmlStreamWriter>

namespace FastCgiQt
{
	/** Base class for services based on XSL-T.
	 *
	 * This provides an XML stream writer for your service to use to write
	 * XML, and actually calling the XSLT processor.
	 *
	 * @warning This class is only available if WITH_XSLT_SUPPORT=ON is
	 * passed to CMake when building FastCgiQt. The default is off, as
	 * this requires Qt 4.5, which hasn't been released yet.
	 *
	 * @warning This class must only be used for one request at a time - for example,
	 * via a Responder subclass and Spawners.
	 *
	 * @ingroup services
	 */
	class XsltService : public Service
	{
		Q_OBJECT
		public:
			XsltService(QObject* parent = 0);
			virtual ~XsltService();
		protected:
			virtual void dispatchUncachedRequest(const QByteArray& suffix);
			/// Constructor.

			/** Add an XSLT snippet to the XSLT source string.
			 *
			 * Files added via this method will be concatenated.
			 *
			 * @deprecated This is a temporary work around for the
			 * 	lack of xsl:include/xsl:import support in Qt 4.5
			 *
			 * @see setXsltUrl(const QString&)
			 * @see setXSltUrl(const QUrl&)
			 */
			void addXsltFile(const QString& path);
			/// Set the source XSLT file.
			/// @see setXsltUrl(const QUrl&)
			void setXsltUrl(const QString& path);
			/// Set the source XSLT URL.
			/// @see setXsltUrl(const QString&)
			void setXsltUrl(const QUrl& url);
			/// Set a variable for the XSLT processor.
			void setVariable(const QString& name, const QVariant& value);

			/// Whether the output XML is formatted nicely.
			bool isPrettyPrintingEnabled() const;
			/// Set whether or not to format the output XML nicely.
			void setPrettyPrintingEnabled(bool);

			/// Device to write XML to
			QIODevice* xmlDevice();
		private:
			class Private;
			Private* d;
	};
}

#endif
