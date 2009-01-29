#include "Dumper.h"
#include <FastCgiQt/Manager.h>

#include <QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("Dumper");

	FastCgiQt::Manager manager(&Dumper::create);

	return application.exec();
}
