#pragma once

#include <FastCgiQt/Request.h>

class PrefixExample : public QObject
{
	Q_OBJECT;
	public:
		PrefixExample(QObject* parent = 0);
	public slots:
		void foo(FastCgiQt::Request*);
		void bar(FastCgiQt::Request*);
		void baz(FastCgiQt::Request*);
};
