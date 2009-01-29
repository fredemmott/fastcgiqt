
#include "GetGreeter.h"
#include <FastCgiQt/Manager.h>

#include <QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("GetGreeter");

	FastCgiQt::Manager manager(&GetGreeter::create);

	return application.exec();
}
