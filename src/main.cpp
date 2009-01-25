
#include "DebugHandler.h"
#include "Listener.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("FastCgiQt");

	FastCgiQt::DebugHandler handler;
	FastCgiQt::Listener listener;

	return application.exec();
}
