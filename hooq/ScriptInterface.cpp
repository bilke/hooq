#include "ScriptInterface.h"

#include "RemoteObjectPrototype.h"

#include <QDebug>

ScriptInterface* ScriptInterface::m_instance(0);

ScriptInterface::ScriptInterface()
: QObject()
{
}

ScriptInterface* ScriptInterface::instance()
{
	if(!m_instance)
	{
		m_instance = new ScriptInterface();
	}
	return m_instance;
}

QScriptValue ScriptInterface::usleep(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(engine);
	const int msec = context->argument(0).toInteger();
	emit instance()->usleep(msec);
	return QScriptValue();
}

QScriptValue ScriptInterface::objectFromPath(QScriptContext* context, QScriptEngine* engine)
{
	const QString path = context->argument(0).toString();
	RemoteObjectPrototype* object = new RemoteObjectPrototype(path);
	emit instance()->newRemoteObject(object);
	return engine->newQObject(object, QScriptEngine::ScriptOwnership);
}
