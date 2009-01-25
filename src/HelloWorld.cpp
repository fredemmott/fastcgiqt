#include "HelloWorld.h"

#include <QDebug>

void HelloWorld::respond()
{
	out() << "hello, world" << endl;
	qDebug() << "Sent hello, world";
}
