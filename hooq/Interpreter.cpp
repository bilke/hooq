#include "Interpreter.h"

#include "ScriptInterface.h"

#include <QDebug>
#include <QFile>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QStringList>

Interpreter::Interpreter(QObject* parent)
: QObject(parent)
, m_engine(new QScriptEngine(this))
, m_debugger(new QScriptEngineDebugger(this))
{
	if(m_engine->importExtension("qt.core").isError())
	{
		qDebug() << Q_FUNC_INFO << "Failed to load qt.core QScript plugin";
	}
	else
	{
		qDebug() << "Loaded qt.core plugin";
	}

	m_debugger->attachTo(m_engine);
	m_engine->globalObject().setProperty(
		"usleep",
		m_engine->newFunction(ScriptInterface::usleep, 1)
	);
	m_engine->globalObject().setProperty(
		"objectFromPath",
		m_engine->newFunction(ScriptInterface::objectFromPath, 1)
	);
}

void Interpreter::run(const QString& script)
{
	QFile file(script);
	file.open(QIODevice::ReadOnly);
	Q_ASSERT(file.isOpen() && file.isReadable());
	const QString text = QString::fromUtf8(file.readAll());
	file.close();

	m_engine->evaluate(text, script);
}
