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
#include "WindowsInjector.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTimer>

#include "../injectedHooq/WinDll.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

void WindowsInjector::startAndAttach(const QString& application, const QStringList& arguments)
{
	delete m_process;
	QDir::setCurrent(QFileInfo(application).canonicalPath());
	m_process = new QProcess(this);

	m_attachTimer = new QTimer(this);
	m_attachTimer->setSingleShot(true);
	m_attachTimer->setInterval(500);
	connect(
		m_attachTimer,
		SIGNAL(timeout()),
		this,
		SLOT(attachToSpawnedProcess())
	);

	connect(
		m_process,
		SIGNAL(started()),
		m_attachTimer,
		SLOT(start())
	);

	connect(
		m_process,
		SIGNAL(finished(int, QProcess::ExitStatus)),
		SIGNAL(finished(int))
	);

	m_process->start(application, arguments);
}

void WindowsInjector::attachToSpawnedProcess()
{
	if(m_process->state() == QProcess::NotRunning)
	{
		// TODO: Propogate to UI
		qDebug() << "Failed to start process:" << m_process->error();
	}
	else
	{
		attach(m_process->pid());
	}
}

void WindowsInjector::attach(Q_PID processId)
{
	wchar_t path[_MAX_PATH];
	const int pathLength = libraryPath().toWCharArray(path);
	path[pathLength] = 0;

	HMODULE library = ::LoadLibraryW(path);
	FARPROC hook = ::GetProcAddress(library, "installHooq");

	typedef void(*Installer)(HINSTANCE, DWORD);
	Installer installer = reinterpret_cast<Installer>(hook);
	(*installer)(library, processId->dwThreadId);
}

QString WindowsInjector::libraryPath()
{
	return QDir::toNativeSeparators(QFileInfo(QCoreApplication::applicationDirPath(), "injectedHooq.dll").canonicalFilePath());
}


} // namespace
