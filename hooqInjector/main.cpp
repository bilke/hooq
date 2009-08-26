#include "GdbInjector.h"
#include "RemoteLogger.h"
#include "RemotePlayback.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>

using namespace Hooq;

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	Injector::Action mode;
	if(app.arguments().contains("--record"))
	{
		mode = Injector::Record;
	}
	else if(app.arguments().contains("--play"))
	{
		mode = Injector::Replay;
	}
	else
	{
		QTextStream cerr(stderr);
		cerr << "Syntax: " << app.arguments().first() << " {--record|--play} [--spam]" << endl;
		return 1;
	}


	QFile log("eventLog.txt");
	if(mode == Injector::Replay)
	{
		log.open(QIODevice::ReadOnly);
	}
	else
	{
		log.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Unbuffered);
	}

	GdbInjector injector;

	RemoteLogger logger;
	RemotePlayback player;

	if(mode == Injector::Replay)
	{
		qDebug() << "Playback!";
		player.start("../demo/demo", &log, &injector);
	}
	else
	{
		qDebug() << "Logging!";
		logger.start("../demo/demo", &log, &injector);
	}

	return app.exec();
}
