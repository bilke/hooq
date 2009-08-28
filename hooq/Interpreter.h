#pragma once

class RemoteObjectPrototype;

#include <QObject>
#include <QXmlStreamWriter>

class QPoint;
class QScriptEngine;
class QScriptEngineDebugger;
 
class Interpreter : public QObject, private QXmlStreamWriter
{
	Q_OBJECT;
	public:
		Interpreter(QObject* parent);
		void run(const QString& script, QIODevice* xmlOut);
	private slots:
		void connectRemoteObject(RemoteObjectPrototype*);
		void writeMouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeSleep(int msec);
	private:
		QScriptEngine* m_engine;
		QScriptEngineDebugger* m_debugger;
};
