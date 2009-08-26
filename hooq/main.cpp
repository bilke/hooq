#include "GdbInjector.h"
#include "RemoteLogger.h"
#include "RemotePlayback.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QTextStream>

using namespace Hooq;

enum Mode
{
	Record,
	Replay
};

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	Mode mode;
	if(app.arguments().contains("--record"))
	{
		mode = Record;
	}
	else if(app.arguments().contains("--play"))
	{
		mode = Replay;
	}
	else
	{
		QTextStream cerr(stderr);
		cerr << "Syntax: " << app.arguments().first() << " {--record|--play} [--spam]" << endl;
		return 1;
	}


	QFile log("eventLog.txt");
	if(mode == Replay)
	{
		log.open(QIODevice::ReadOnly);
	}
	else
	{
		log.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Unbuffered);
	}

	GdbInjector injector;

	QObject::connect(
		&injector,
		SIGNAL(finished(int)),
		&app,
		SLOT(quit())
	);

	RemoteLogger logger;
	RemotePlayback player;

	if(mode == Replay)
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
