#ifndef _FASTCGI_QT_XSLT_SERVICE_PRIVATE_H
#define _FASTCGI_QT_XSLT_SERVICE_PRIVATE_H

namespace FastCgiQt
{
	/** @internal
	 * @brief data members for XsltService.
	 */
	class XsltService::Private
	{
		public:
			/// Initialise values
			Private();
			/// Enum specifying possible sources of XSLT.
			enum XsltSource
			{
				NoXslt,    ///< No XSLT
				FileXslt,  ///< the file pointed at by @ref xsltUrl
				StringXslt ///< the @ref xslt QByteArray member
			};
			/// Where the XSLT comes from.
			XsltSource source;
			/// A URL containing the location of some XSLT.
			QUrl xsltUrl;
			/// A byte array containing some XSLT.
			QByteArray xslt;
			/// A string containing application-generated XML.
			QString xml;
			/// Whether or not to pretty-print (indent) the XML output.
			bool prettyPrint;
			/// Variables passed to the XSL-T processor.
			QVariantMap variables;
	};
}

#endif
