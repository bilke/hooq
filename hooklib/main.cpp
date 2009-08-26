#include "GdbInjector.h"
#include "RemoteLogger.h"

#include <QCoreApplication>
#include <QFile>

using namespace Hooq;

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	QFile log("eventLog.txt");
	log.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Unbuffered);

	GdbInjector injector;
	RemoteLogger logger;
	logger.start("../hooked/hooked", &log, &injector);

	return app.exec();
}
