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
	m_gdbStream << "break _start" << endl; // C entry point - after main libraries have been loaded
	m_gdbStream << "run" << endl; // run until we hit it, and therefore Qt shared libraries are loaded
	m_gdbStream << "break QCoreApplication::exec()" << endl; // now, we can set this breakpoint...
	m_gdbStream << "continue" << endl;
	m_gdbStream << QString("call __dlopen(\"%1\", %2)").arg(libraryPath()).arg(QString::number(RTLD_NOW)) << endl; // load our library
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
