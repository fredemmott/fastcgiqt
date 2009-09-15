#include "UnsignedByteArray.h"

namespace FastCgiQt
{
	UnsignedByteArray::UnsignedByteArray(const QByteArray& array)
	: QByteArray(array)
	{
	}

	quint8 UnsignedByteArray::at(int i) const
	{
		return static_cast<quint8>(QByteArray::at(i));
	}

	quint8 UnsignedByteArray::operator[](int i) const
	{
		return at(i);
	}
};
