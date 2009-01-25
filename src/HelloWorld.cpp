#include "HelloWorld.h"

void HelloWorld::respond()
{
	out() << "Content-type: text/html\r\n\r\n" << flush;
	out() << "hello, world" << endl;
}
