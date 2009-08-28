#include "Interpreter.h"

#include "RemoteObjectPrototype.h"
#include "ScriptInterface.h"

#include <QDebug>
#include <QFile>
#include <QPoint>
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

	connect(
		ScriptInterface::instance(),
		SIGNAL(usleep(int)),
		SLOT(writeSleep(int))
	);

	connect(
		ScriptInterface::instance(),
		SIGNAL(newRemoteObject(RemoteObjectPrototype*)),
		SLOT(connectRemoteObject(RemoteObjectPrototype*))
	);
}

void Interpreter::connectRemoteObject(RemoteObjectPrototype* object)
{
	connect(
		object,
		SIGNAL(mouseMoveEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers)),
		SLOT(writeMouseMoveEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers))
	);
}

void Interpreter::writeMouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	writeStartElement("mouseMove");
	writeAttribute("x", QString::number(position.x()));
	writeAttribute("y", QString::number(position.y()));
	writeAttribute("button", QString::number(button));
	writeAttribute("buttons", QString::number(buttons));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("target", path);
	writeEndElement();
}

void Interpreter::run(const QString& script, QIODevice* xmlOut)
{
	setDevice(xmlOut);

	QFile file(script);
	file.open(QIODevice::ReadOnly);
	Q_ASSERT(file.isOpen() && file.isReadable());
	const QString text = QString::fromUtf8(file.readAll());
	file.close();

	writeStartDocument();
	writeStartElement("hooq");

	m_engine->evaluate(text, script);

	writeEndElement(); // hooq
	writeEndDocument();
}

void Interpreter::writeSleep(int msec)
{
	writeTextElement("msec", QString::number(msec));
}