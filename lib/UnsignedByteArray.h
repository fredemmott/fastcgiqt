#pragma once

#include <QByteArray>

namespace FastCgiQt
{
	/// Read-only access to a QByteArray as unsigned data.
	class UnsignedByteArray : public QByteArray
	{
		public:
			UnsignedByteArray(const QByteArray&);
			quint8 at(int i) const;
			quint8 operator[](int i) const;
		private:
			// Disallow all other overloads
			using QByteArray::at;
			using QByteArray::operator[];
	};
};
