
#include "GetGreeter.h"
#include <FastCgiQt/DebugHandler.h>
#include <FastCgiQt/Manager.h>

#include <QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("GetGreeter");

	FastCgiQt::DebugHandler handler; // not neccessary - redirects q{Debug,Fatal,Critical} to syslog

	FastCgiQt::Manager manager(&GetGreeter::instance);

	return application.exec();
}
