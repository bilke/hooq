#include "WindowsInjector.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QProcess>

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
	wchar_t path[_MAX_PATH];
	void* remotePathAddress;
	HMODULE hKernel32 = ::GetModuleHandle(L"Kernel32");

	const int pathLength = libraryPath().toWCharArray(path);
	path[pathLength] = 0;

	// Grab some memory in the remote process to shove the path to the hooq DLL in
	remotePathAddress = ::VirtualAllocEx(
		processId->hProcess,
		NULL,
		sizeof(path),
		MEM_COMMIT,
		PAGE_READWRITE
	);
	// Copy the path to that address
	::WriteProcessMemory(
		processId->hProcess,
		remotePathAddress,
		static_cast<void*>(path),
		sizeof(path),
		NULL
	);

	// Find out where 'LoadLibrary' lives
	LPTHREAD_START_ROUTINE loadLibraryAddress = reinterpret_cast<LPTHREAD_START_ROUTINE>(::GetProcAddress(hKernel32, "LoadLibraryW"));
	Q_ASSERT(loadLibraryAddress);

	// Load the library in the remote process

	::CreateRemoteThread(
		processId->hProcess,
		NULL, // default attributes
		0, // default stack size
		loadLibraryAddress, // function to call
		remotePathAddress, // argument (wchar_t* to our library path)
		0, // no creation flags
		NULL // no thread identifier
	);

}

QString WindowsInjector::libraryPath()
{
	return QFileInfo("injectedHooq.dll").canonicalFilePath();
}


} // namespace
