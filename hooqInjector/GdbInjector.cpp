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
#include "GdbInjector.h"

#include <QCoreApplication>
#include <QDebug>
#include <QProcess>

#include <dlfcn.h> // no functions used, but need to pass RTLD_NOW to gdb

namespace Hooq
{

GdbInjector::GdbInjector(QObject* parent)
: Injector(parent)
, m_gdb(0)
{
}

GdbInjector::~GdbInjector()
{
}

void GdbInjector::startAndAttach(const QString& application)
{
	delete m_gdb;
	m_gdb = new QProcess(this);
	const QStringList arguments = QStringList()
		<< "-return-child-result"
		<< "-interpreter" << "mi" // machine interface
		<< application
	;

	connect(
		m_gdb,
		SIGNAL(started()),
		SLOT(startProcess())
	);
	connect(
		m_gdb,
		SIGNAL(finished(int, QProcess::ExitStatus)),
		SIGNAL(finished(int))
	);

	if(QCoreApplication::arguments().contains("--spam"))
	{
		connect(
			m_gdb,
			SIGNAL(readyReadStandardOutput()),
			SLOT(printGdbOutput())
		);
		connect(
			m_gdb,
			SIGNAL(readyReadStandardError()),
			SLOT(printGdbError())
		);
	}
	m_gdb->start("gdb", arguments);
}

void GdbInjector::startProcess()
{
	Q_ASSERT(m_gdb->state() == QProcess::Running);
	Q_ASSERT(m_gdb->isWritable());
	m_gdbStream.setDevice(m_gdb);
	m_gdbStream << "set auto-solib-add off" << endl; // avoid loading symbols for shared libraries
	                                                 // that we do not need to interact with from GDB
	m_gdbStream << "break _start" << endl; // C entry point - after main libraries have been loaded
	m_gdbStream << "run" << endl; // run until we hit it, and therefore Qt shared libraries are loaded
	m_gdbStream << "sharedlibrary libdl" << endl; // load the libdl library so that we can call __dlopen()
	m_gdbStream << "sharedlibrary libc" << endl;  // load the libc library so that we can call __dlopen()
	m_gdbStream << "sharedlibrary libQtCore" << endl; // load QtCore for breaking on QCoreApplication::exec()
	m_gdbStream << "break QCoreApplication::exec()" << endl; // now, we can set this breakpoint...
	m_gdbStream << "continue" << endl;
	m_gdbStream << QString("call __dlopen(\"%1\", %2)").arg(libraryPath()).arg(QString::number(RTLD_NOW)) << endl; // load our library
	m_gdbStream << "sharedlibrary hooq" << endl; // load the hooq injector library so that we can call startHooq()
	m_gdbStream << "call startHooq()" << endl; // install our plugin (which required QCoreApplication setup)
	m_gdbStream << "continue" << endl; // run the app
	m_gdbStream << "quit" << endl; // after the application has exited, quit gdb
}

void GdbInjector::attach(Q_PID processId)
{
	qDebug() << Q_FUNC_INFO << "TODO";
	Q_UNUSED(processId);
}

void GdbInjector::printGdbOutput()
{
	// SPAM SPAM SPAM
	m_gdb->setReadChannel(QProcess::StandardOutput);
	qDebug() << m_gdbStream.readAll();
}

void GdbInjector::printGdbError()
{
	// SPAM SPAM SPAM
	m_gdb->setReadChannel(QProcess::StandardError);
	qDebug() << m_gdbStream.readAll();
}

QString GdbInjector::libraryPath()
{
	// XXX FIXME XXX
	return QCoreApplication::applicationDirPath() + "/../injectedHooq/libinjectedHooq.so.1.0.0";
}


} // namespace
