#include "GdbInjector.h"
#include "RemoteLogger.h"
#include "RemotePlayback.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
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

	QString application = "../demo/demo";
	if(!app.arguments().last().startsWith("--"))
	{
		application = app.arguments().last();
	}

	QDir::setCurrent(QFileInfo(application).absolutePath());
	

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
		player.start(application, &log, &injector);
	}
	else
	{
		logger.start(application, &log, &injector);
	}

	return app.exec();
}
