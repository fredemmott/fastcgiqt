#include "EmbedExample.h"

#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(true);
	EmbedExample window;
	window.show();
	return app.exec();
}
