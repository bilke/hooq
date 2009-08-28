#pragma once

#include <QScriptContext>
#include <QScriptEngine>
#include <QScriptValue>

class RemoteObjectPrototype;

class ScriptInterface : public QObject
{
	Q_OBJECT;
	public:
		static ScriptInterface* instance();

		static QScriptValue usleep(QScriptContext*, QScriptEngine*);
		static QScriptValue objectFromPath(QScriptContext*, QScriptEngine*);
	signals:
		void usleep(int msec);
		void newRemoteObject(RemoteObjectPrototype*);
	private:
		ScriptInterface();

		static ScriptInterface* m_instance;
};
