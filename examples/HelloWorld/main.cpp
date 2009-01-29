#include "HelloWorld.h"
#include <FastCgiQt/Manager.h>

#include <QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("HelloWorld");

	FastCgiQt::Manager manager(&HelloWorld::instance);

	return application.exec();
}
