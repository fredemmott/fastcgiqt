#ifndef _FASTCGI_QT_STANDARD_OUTPUT_RECORD_H
#define _FASTCGI_QT_STANDARD_OUTPUT_RECORD_H

#include <QByteArray>

namespace FastCgiQt
{
	/** @internal
	 * @brief Class providing function to create a FastCGI STDOUT record.
	 *
	 * These records contain data to be sent to the client.
	 */
	class StandardOutputRecord
	{
		public:
			/** Create a FastCGI STDOUT record.
			 *
			 * @param requestId is the FastCGI ID for the request
			 * 	that this data is for.
			 * @param data is the data to send to the client.
			 *
			 * @returns a FastCGI STDOUT record in a QByteArray.
			 */
			static QByteArray create(quint16 requestId, QByteArray data);
	};
};

#endif
