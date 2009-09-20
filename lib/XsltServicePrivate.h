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
