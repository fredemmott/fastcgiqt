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
#ifndef _FASTCGI_QT_BEGIN_REQUEST_RECORD_H
#define _FASTCGI_QT_BEGIN_REQUEST_RECORD_H

#include "EnumHelpers.h"
#include "fastcgi.h"

#include <QFlags>
#include <QObject>

namespace FastCgiQt
{
	class RecordHeader;
	/** @internal
	 * @brief Class to parse the data of a FastCGI begin-request record.
	 *
	 * This will parse FastCGI begin-request record data, and provide access
	 * to the information in it through more convenient types.
	 */
	class BeginRequestRecord
	{
		Q_GADGET
		public:
			Q_ENUMS(Role Flags)
			/// FastCGI request roles.
			enum Role
			{
				ResponderRole  = FCGI_RESPONDER,  ///< Respond to a client request.
				AuthorizerRole = FCGI_AUTHORIZER, ///< Provide authorization services.
				FilterRole     = FCGI_FILTER,     ///< Filter data.

				UnknownRole,	                  ///< The first as-yet unknown role.
				FirstRole = ResponderRole         ///< Marker for the lowest-value Role.
			};

			/// Request flags.
			enum Flag
			{
				KeepConnection = FCGI_KEEP_CONN, ///< If this flag is not set, the socket will be closed once this request has been dealt with.

				UnknownFlag,                     ///< The lowest-value unknown flag
				FirstFlag = KeepConnection       ///< Marker for the lowest-value Flag.
			};
			Q_DECLARE_FLAGS(Flags, Flag)

			/** Create a BeginRequestRecord.
			 *
			 * @param header is the header of this record.
			 * @param FCGI_BeginRequestBody is the struct to parse.
			 */
			BeginRequestRecord(const RecordHeader& header, const FCGI_BeginRequestBody&);

			/// The role of this request.
			Role role() const;
			/// The flags for this request.
			Flags flags() const;
		private:
			Role m_role;
			Flags m_flags;
	};
};

DECLARE_DEBUG_ENUM(FastCgiQt::BeginRequestRecord, Role);
Q_DECLARE_OPERATORS_FOR_FLAGS(FastCgiQt::BeginRequestRecord::Flags);

#endif
