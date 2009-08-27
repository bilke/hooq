/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "RemoteLogger.h"
#include "RemotePlayback.h"

#include "PlatformInjector.h"

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

	PlatformInjector injector;

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