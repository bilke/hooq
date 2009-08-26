#include "GdbInjector.h"

#include <QCoreApplication>

using namespace Hooq;

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	GdbInjector injector;
	injector.startAndAttach("../hooked/hooked", Injector::Record);

	return app.exec();
}
