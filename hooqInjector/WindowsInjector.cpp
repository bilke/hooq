#include "WindowsInjector.h"

#include <QCoreApplication>
#include <QDebug>
#include <QProcess>

namespace Hooq
{

WindowsInjector::WindowsInjector(QObject* parent)
: Injector(parent)
, m_process(0)
{
}

WindowsInjector::~WindowsInjector()
{
}

void WindowsInjector::startAndAttach(const QString& application)
{
	delete m_process;
	m_process = new QProcess(this);

	connect(
		m_process,
		SIGNAL(started()),
		SLOT(attachToSpawnedProcess())
	);

	connect(
		m_process,
		SIGNAL(finished(int, QProcess::ExitStatus)),
		SIGNAL(finished(int))
	);

	m_process->start(application);
}

void WindowsInjector::attachToSpawnedProcess()
{
	attach(m_process->pid());
}

void WindowsInjector::attach(Q_PID processId)
{
	qDebug() << Q_FUNC_INFO << "TODO";
	Q_UNUSED(processId);
}

QString WindowsInjector::libraryPath()
{
	// XXX FIXME XXX
	return QCoreApplication::applicationDirPath() + "/../../injectedHooq/debug/injectedHooq.dll";
}


} // namespace
