#include "EnumHelpers.h"

const char* nameFromEnumHelper(const QMetaObject& so, const char* enumerator, int value)
{
	int enumIndex = so.indexOfEnumerator(enumerator);
	if(enumIndex == -1)
	{
		qFatal("Class '%s' doesn't contain enum '%s'.", so.className(), enumerator);
	}
	return so.enumerator(enumIndex).valueToKey(value);
}
