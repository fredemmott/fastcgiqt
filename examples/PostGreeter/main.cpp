
#include "PostGreeter.h"
#include <FastCgiQt/Manager.h>

#include <QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("PostGreeter");

	FastCgiQt::Manager manager(&PostGreeter::create);

	return application.exec();
}
