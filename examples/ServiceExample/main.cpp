#include "ServiceExample.h"
#include <FastCgiQt/Manager.h>

#include <QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("ServiceExample");

	FastCgiQt::Manager manager(&ServiceExample::create);
	return application.exec();
	return 0;
}
