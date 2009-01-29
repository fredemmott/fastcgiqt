#ifndef _FASTCGI_QT_INPUT_DEVICE_H
#define _FASTCGI_QT_INPUT_DEVICE_H

#include <QIODevice>

namespace FastCgiQt
{
	/** @internal
	 * @brief QIODevice providing a stream for POST data.
	 *
	 * This is a read-only stream device, which provides access to POST
	 * data.
	 */
	class InputDevice : public QIODevice
	{
		Q_OBJECT
		public:
			/// Create an input device.
			InputDevice(QObject* parent = NULL);
			/** Try to read data from the buffer.
			 * @returns the number of bytes read, or -1 if an error
			 * 	occured.
			 */
			qint64 readData(char* data, qint64 maxSize);
			/** Try (and fail) to write data.
			 *
			 * @returns -1
			 */
			qint64 writeData(const char* data, qint64 maxSize);

			/** Wait until there is POST data to be read.
			 *
			 * @returns true if there is data to read.
			 * @returns false if more than @param msecs milliseconds
			 * 	pass without more data arriving, or, if all the
			 * 	POST data has already been received.
			 */
			bool waitForReadyRead(int msecs);

			/// Add data to the buffer.
			void appendData(const QByteArray& data);
		private:
			/// The current position in the data buffer.
			quint64 m_position;
			/// Whether or not all data has been received.
			bool m_atEnd;
			/// Buffer with all the POST data.
			QByteArray m_data;
	};
}

#endif
