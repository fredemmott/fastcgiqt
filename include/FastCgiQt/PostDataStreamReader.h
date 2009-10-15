#pragma once

#include "Request.h"

class QByteArray;
class QIODevice;
namespace FastCgiQt
{
	class PostDataStreamReader
	{
		public:
			enum TokenType
			{
				Invalid       = 0,
				NoToken       = 1,
				StartData     = 2,
				VariableName  = 3,
				VariableValue = 4,
				FileHeader    = 5,
				FileValue     = 6,
				EndData       = 7
			};
			PostDataStreamReader(FastCgiQt::Request* request);
			~PostDataStreamReader();

			bool atEnd() const;
			TokenType tokenType() const;
			TokenType readNext();

			/** The name of the variable.
			 *
			 * Valid for VariableName, VariableValue, FileHeader, and FileValue
			 */
			QString variableName() const;

			/** Contents of the current variable/file.
			 *
			 * This is a streaming device.
			 *
			 * Valid for VariableValue and FileValue.
			 */
			QIODevice* content() const;

			/** Name of the file being uploaded.
			 *
			 * This is provided by the user-agent, so should be sanitised.
			 *
			 * Valid for FileHeader.
			 */
			QString filename() const;
			/** MIME type of the file being uploaded.
			 *
			 * This is provided by the user-agent.
			 *
			 * Valid for FileHeader.
			 */
			QString mimeType() const;
		private:
			class Private;
			Private* d;
	};
};
