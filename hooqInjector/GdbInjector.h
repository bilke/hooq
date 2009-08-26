#pragma once

#include "Injector.h"

#include <QTextStream>

namespace Hooq
{

/// gdb-based DLL injector for Linux/OSX
class GdbInjector : public Injector
{
	Q_OBJECT
	public:
		GdbInjector(QObject* parent = 0);
		virtual ~GdbInjector();

		virtual void startAndAttach(const QString& application);
		virtual void attach(Q_PID processId);
	private slots:
		void printGdbOutput();
		void printGdbError();
		void startProcess();
	private:
		static QString libraryPath();
		QProcess* m_gdb;
		QTextStream m_gdbStream;
};

} // namespace
