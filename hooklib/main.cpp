#include "GdbInjector.h"
#include "RemoteLogger.h"

#include <QCoreApplication>

using namespace Hooq;

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	GdbInjector injector;
	RemoteLogger logger;
	logger.start("../hooked/hooked", 0, &injector);

	return app.exec();
}
