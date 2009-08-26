#pragma once

#include "InjectorImport.h"

#include "Injector.h"

namespace Hooq
{

/// Windows-based DLL injector for Linux/OSX
class HOOQ_INJECTOR_EXPORT WindowsInjector : public Injector
{
	Q_OBJECT
	public:
		WindowsInjector(QObject* parent = 0);
		virtual ~WindowsInjector();

		virtual void startAndAttach(const QString& application);
		virtual void attach(Q_PID processId);
	private slots:
		void attachToSpawnedProcess();
	private:
		static QString libraryPath();
		QProcess* m_process;
};

} // namespace
