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

#include "../common/Communication.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QTcpServer>
#include <QTextStream>

using namespace Hooq;

enum Mode
{
	Record,
	Replay
};

void printSyntax()
{
	QTextStream cerr(stderr);
	cerr << "Syntax: " << QCoreApplication::arguments().first() << " {--record|--play} [--spam] applicationPath" << endl;
}

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
		printSyntax();
		return 1;
	}

	const QString application = QDir::fromNativeSeparators(app.arguments().last());
	if(!QFile::exists(application))
	{
		printSyntax();
		return 1;
	}

	QFile log("eventLog.xml");
	if(mode == Replay)
	{
		log.open(QIODevice::ReadOnly);
	}
	else
	{
		log.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Unbuffered);
	}

	QDir::setCurrent(QFileInfo(application).absolutePath());

	PlatformInjector injector;

	QObject::connect(
		&injector,
		SIGNAL(finished(int)),
		&app,
		SLOT(quit())
	);

	QTcpServer server;
	server.listen(QHostAddress::LocalHost, Hooq::Communication::serverPort());

	RemoteLogger logger(&server);
	RemotePlayback player(&server);

	if(mode == Replay)
	{
		player.start(application, QStringList(), &log, &injector);
	}
	else
	{
		logger.start(application, QStringList(), &log, &injector);
	}

	return app.exec();
}
