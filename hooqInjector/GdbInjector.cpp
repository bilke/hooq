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

void GdbInjector::startAndAttach(const QString& application, Action action)
{
	delete m_gdb;
	m_gdb = new QProcess(this);
	const QStringList arguments = QStringList()
		<< "-return-child-result"
		<< "-interpreter" << "mi" // machine interface
		<< application
	;
	if(action == Record)
	{
		connect(
			m_gdb,
			SIGNAL(started()),
			SLOT(startProcessWithLogger())
		);
	}
	else
	{
		Q_ASSERT(action == Replay);
		connect(
			m_gdb,
			SIGNAL(started()),
			SLOT(startProcessWithPlayback())
		);
	}

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

void GdbInjector::startProcessWithLogger()
{
	startProcess("QApplication::exec()", loggerLibrary(), "startHooq()");
}

void GdbInjector::startProcessWithPlayback()
{
	startProcess("QApplication::exec()", playbackLibrary(), "startHooq()");
}

void GdbInjector::startProcess(const QString& breakPoint, const QString& library, const QString& call)
{
	Q_ASSERT(m_gdb->state() == QProcess::Running);
	Q_ASSERT(m_gdb->isWritable());
	m_gdbStream.setDevice(m_gdb);
	m_gdbStream << "break _start" << endl; // C entry point - after main libraries have been loaded
	m_gdbStream << "run" << endl; // run until we hit it, and therefore Qt shared libraries are loaded
	m_gdbStream << QString("break %1").arg(breakPoint) << endl; // now, we can set this breakpoint...
	m_gdbStream << "continue" << endl;
	m_gdbStream << QString("call __dlopen(\"%1\", %2)").arg(library).arg(QString::number(RTLD_NOW)) << endl; // load our library
	m_gdbStream << QString("call %1").arg(call) << endl; // install our plugin (which required QCoreApplication setup)
	m_gdbStream << "continue" << endl; // run the app
	m_gdbStream << "quit" << endl; // after the application has exited, quit gdb
}

void GdbInjector::attach(int processId, Action action)
{
	qDebug() << Q_FUNC_INFO << "TODO";
	Q_UNUSED(processId);
	Q_UNUSED(action);
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

QString GdbInjector::loggerLibrary()
{
	// XXX FIXME XXX
	return QCoreApplication::applicationDirPath() + "/../injectedHooq/libinjectedHooq.so.1.0.0";
//	return QCoreApplication::applicationDirPath() + "/../hook/libhook.so.1.0.0";
}

QString GdbInjector::playbackLibrary()
{
	// XXX FIXME XXX
	return QCoreApplication::applicationDirPath() + "/../injectedHooq/libinjectedHooq.so.1.0.0";
//	return QCoreApplication::applicationDirPath() + "/../player/libplayer.so.1.0.0";
}


} // namespace
