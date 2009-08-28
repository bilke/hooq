#pragma once

class RemoteObjectPrototype;

#include <QObject>
#include <QXmlStreamWriter>

class QKeySequence;
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
		void writeKeyPressEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void writeKeyReleaseEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void writeMouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeMousePressEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeMouseReleaseEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeShortcutEvent(const QString& path, const QKeySequence& sequence, int id, bool ambiguous);
		void writeSleep(int msec);
		void writeWheelEvent(const QString& path, const QPoint& position, int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::Orientation orientation);
	private:
		void writeKeyAttributes(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void writeMouseAttributes(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);

		QScriptEngine* m_engine;
		QScriptEngineDebugger* m_debugger;
};
