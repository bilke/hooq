#pragma once

#include <QObject>

class QScriptEngine;
class QScriptEngineDebugger;
 
class Interpreter : public QObject
{
	Q_OBJECT;
	public:
		Interpreter(QObject* parent);
		void run(const QString& script);
	private:
		QScriptEngine* m_engine;
		QScriptEngineDebugger* m_debugger;
};
