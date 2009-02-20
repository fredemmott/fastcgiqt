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
	 * 	passed to CMake when building FastCgiQt. The default is off, as
	 * 	this requires Qt 4.5, which hasn't been released yet.
	 */
	class XsltService : public Service
	{
		Q_OBJECT
		private:
			class Private;
			Private* d;
		public:
			virtual ~XsltService();
		protected:
			/// Main implementation
			virtual void dispatchUncachedRequest(const QString& urlFragment);
			/// Constructor.
			XsltService(const FastCgiQt::Request&, QObject* parent = NULL);

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

			/// Stream writer for writing the application-generated XML to.
			QXmlStreamWriter xmlOut;
	};
}

#endif
