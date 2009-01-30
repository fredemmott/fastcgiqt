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
#ifndef _FASTCGI_QT_ENUM_HELPERS_H
#define _FASTCGI_QT_ENUM_HELPERS_H

/** Helper functions for dealing with enums.
 * @internal @file
 *
 * This requires the enum in question to be declared with Q_ENUMS.
 */

#include <QDebug>
#include <QMetaEnum>
#include <QMetaObject>

/** Get the name of an enum value from a static meta object.
 * @param so is a staticMetaObject.
 * @param enumerator is an enum name;
 * @param value is a value in this enum.
 * @returns a const char* containing the key of an enum.
 */
template<class T> const char* nameFromEnumHelper(const QMetaObject& so, const char* enumerator, T value)
{
	int enumIndex = so.indexOfEnumerator(enumerator);
	if(enumIndex == -1)
	{
		qFatal("Class '%s' doesn't contain enum '%s'.", so.className(), enumerator);
	}
	return so.enumerator(enumIndex).valueToKey(static_cast<int>(value));
}

/** Get the value of an enum from it's key from a static meta object.
 * @param so is a staticMetaObject.
 * @param o is the object/class/namespace containing the enum declaration.
 * @param e is the enum name.
 * @param n is a QString containing the value key.
 * @returns An o::e.
 */
#define ENUM_FROM_NAME_HELPER(so,o,e,n) ( \
	static_cast<o::e>( \
		so.enumerator( \
			so.indexOfEnumerator(#e) \
		).keyToValue(n.toLatin1()) \
	) \
)
/** Get the key of a Qt namespace enum value.
 * @param e is the enum to get a value for, for example, ToolButtonStyle.
 * @param v is the value that a key is wanted for, for example, Qt::ToolButtonTextUnderIcons
 * @returns a const char* containing the key.
 */
#define QT_NAME_FROM_ENUM(e,v) NAME_FROM_ENUM_HELPER(QObject::staticQtMetaObject,e,v)
/** Get the value of a Qt namespace enum value from it's int value.
 * @param e is the enum to get a value for, for example, ToolButtonStyle.
 * @param n is the name of an enum value, for example "ToolButtonTextUnderIcons"
 * @returns A Qt::e value.
 */
#define QT_ENUM_FROM_NAME(e,n) ENUM_FROM_NAME_HELPER(QObject::staticQtMetaObject,Qt,e,n)
/** Get the key of a QObject's enum value.
 * @param o is a QObject
 * @param e is the enum to get a value for, for example, ToolButtonStyle.
 * @param v is the value that a key is wanted for, for example, Qt::ToolButtonTextUnderIcons
 * @returns a const char* containing the key.
 */
#define NAME_FROM_ENUM(o,e,v) nameFromEnumHelper(o::staticMetaObject,#e,v)
/** Get the value of a QObject enum value from it's int value.
 * @param o is a QObject
 * @param e is the enum to get a value for, for example, ToolButtonStyle.
 * @param n is the name of an enum value, for example "ToolButtonTextUnderIcons"
 * @returns A Qt::e value.
 */
#define ENUM_FROM_NAME(o,e,n) ENUM_FROM_NAME_HELPER(o::staticMetaObject,o,e,n)

/// Convenience function for getting a string suitable for writing to qDebug with an enum value.
#define ENUM_DEBUG_STRING(c,e,v) \
	qPrintable(QString("%1 (%2)").arg(static_cast<int>(v)).arg(NAME_FROM_ENUM(c,e,v)))

/** Declare operators neccessary for an enum to be pretty printed by qDebug.
 * @param c is a class
 * @param e is an enum name
 */
#define DECLARE_DEBUG_ENUM(c, e) \
	QDebug operator<<(QDebug dbg, c::e value);
/** Define operators neccessary for an enum to be pretty printed by qDebug.
 * @param c is a class
 * @param e is an enum name
 */
#define DEFINE_DEBUG_ENUM(c, e) \
	QDebug operator<<(QDebug dbg, c::e value) \
	{ \
		dbg.nospace() << ENUM_DEBUG_STRING(c,e,value); \
		return dbg.space(); \
	}
#endif
