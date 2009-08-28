#include "MainWindow.h"

#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	app.setApplicationName("Hooq");
	app.setApplicationVersion("0.1");
	app.setOrganizationDomain("hooq.org");
	app.setOrganizationName("Hooq Developers");

	MainWindow window;
	window.show();

	return app.exec();
}
