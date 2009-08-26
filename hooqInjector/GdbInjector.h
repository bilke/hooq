#pragma once

#include "Injector.h"

#include <QTextStream>

class QProcess;

namespace Hooq
{

/// gdb-based DLL injector for Linux/OSX
class GdbInjector : public Injector
{
	Q_OBJECT
	public:
		GdbInjector(QObject* parent = 0);
		virtual ~GdbInjector();

		virtual void startAndAttach(const QString& application, Action action);
		virtual void attach(int processId, Action action);
	private slots:
		void startProcessWithLogger();
		void startProcessWithPlayback();
		void printGdbOutput();
		void printGdbError();
	private:
		void startProcess(const QString& breakPoint, const QString& library, const QString& call);
		static QString loggerLibrary();
		static QString playbackLibrary();
		QProcess* m_gdb;
		QTextStream m_gdbStream;
};

} // namespace
