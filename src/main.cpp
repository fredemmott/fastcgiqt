
#include "DebugHandler.h"
#include "HelloWorld.h"
#include "Manager.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("FastCgiQt");

	FastCgiQt::DebugHandler handler;
	FastCgiQt::Manager manager(&HelloWorld::instance);

	return application.exec();
}
