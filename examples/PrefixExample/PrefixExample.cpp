#include "PrefixExample.h"

#include <FastCgiQt/PrefixMapper.h>

#include <QTextStream>

PrefixExample::PrefixExample(QObject* parent)
: QObject(parent)
{
}

#define PREFIXEXAMPLE_SLOT(x) \
void PrefixExample::x(FastCgiQt::Request* request) \
{ \
	QTextStream out(request); \
	out << "In function PrefixExample::" << #x << endl; \
	out << "Prefix is: " << FastCgiQt::PrefixMapper::prefix(request) << endl; \
	out << "Suffix is: " << FastCgiQt::PrefixMapper::suffix(request) << endl; \
}

PREFIXEXAMPLE_SLOT(foo);
PREFIXEXAMPLE_SLOT(bar);
PREFIXEXAMPLE_SLOT(baz);
