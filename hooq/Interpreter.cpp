/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "Interpreter.h"

#include "RemoteObjectPrototype.h"
#include "ScriptInterface.h"

#include <QDebug>
#include <QFile>
#include <QKeySequence>
#include <QLocalSocket>
#include <QPoint>
#include <QScriptEngine>
#include <QScriptEngineDebugger>
#include <QStringList>

Interpreter::Interpreter(QObject* parent)
: QObject(parent)
, m_debugger(new QScriptEngineDebugger(this))
, m_engine(new QScriptEngine(this))
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
	connect(
		object,
		SIGNAL(mousePressEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers)),
		SLOT(writeMousePressEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers))
	);
	connect(
		object,
		SIGNAL(mouseReleaseEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers)),
		SLOT(writeMouseReleaseEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers))
	);
	connect(
		object,
		SIGNAL(wheelEvent(QString, QPoint, int, Qt::MouseButtons, Qt::KeyboardModifiers, Qt::Orientation)),
		SLOT(writeWheelEvent(QString, QPoint, int, Qt::MouseButtons, Qt::KeyboardModifiers, Qt::Orientation))
	);
	connect(
		object,
		SIGNAL(keyPressEvent(QString, int, Qt::KeyboardModifiers, QString, bool, ushort)),
		SLOT(writeKeyPressEvent(QString, int, Qt::KeyboardModifiers, QString, bool, ushort))
	);
	connect(
		object,
		SIGNAL(keyReleaseEvent(QString, int, Qt::KeyboardModifiers, QString, bool, ushort)),
		SLOT(writeKeyReleaseEvent(QString, int, Qt::KeyboardModifiers, QString, bool, ushort))
	);
	connect(
		object,
		SIGNAL(shortcutEvent(QString, QKeySequence, int, bool)),
		SLOT(writeShortcutEvent(QString, QKeySequence, int, bool))
	);
}

void Interpreter::writeKeyPressEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count)
{
	writeStartElement("keyPress");
	writeKeyAttributes(path, key, modifiers, text, autorepeat, count);
	writeEndElement();
}

void Interpreter::writeKeyReleaseEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count)
{
	writeStartElement("keyRelease");
	writeKeyAttributes(path, key, modifiers, text, autorepeat, count);
	writeEndElement();
}

void Interpreter::writeMouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	writeStartElement("mouseMove");
	writeMouseAttributes(path, position, button, buttons, modifiers);
	writeEndElement();
}

void Interpreter::writeMousePressEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	writeStartElement("mouseButtonPress");
	writeMouseAttributes(path, position, button, buttons, modifiers);
	writeEndElement();
}

void Interpreter::writeMouseReleaseEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	writeStartElement("mouseButtonRelease");
	writeMouseAttributes(path, position, button, buttons, modifiers);
	writeEndElement();
}

void Interpreter::writeMouseAttributes(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	writeAttribute("x", QString::number(position.x()));
	writeAttribute("y", QString::number(position.y()));
	writeAttribute("button", QString::number(button));
	writeAttribute("buttons", QString::number(buttons));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("target", path);
}

void Interpreter::writeKeyAttributes(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count)
{
	writeAttribute("key", QString::number(key));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("text", text);
	writeAttribute("autorepeat", autorepeat ? "true" : "false");
	writeAttribute("count", QString::number(count));
	writeAttribute("target", path);
}

void Interpreter::writeShortcutEvent(const QString& path, const QKeySequence& sequence, int id, bool ambiguous)
{
	writeStartElement("shortcut");
	writeAttribute("string", sequence.toString());
	writeAttribute("id", QString::number(id));
	writeAttribute("ambiguous", ambiguous ? "true" : "false");
	writeAttribute("target", path);
	writeEndElement();
}

void Interpreter::writeWheelEvent(const QString& path, const QPoint& position, int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::Orientation orientation)
{
	writeStartElement("mouseWheel");
	writeAttribute("x", QString::number(position.x()));
	writeAttribute("y", QString::number(position.y()));
	writeAttribute("delta", QString::number(delta));
	writeAttribute("buttons", QString::number(buttons));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("orientation", QString::number(orientation));
	writeAttribute("target", path);
	writeEndElement();
}

void Interpreter::setScriptPath(const QString& scriptPath)
{
	m_scriptPath = scriptPath;
	QFile file(scriptPath);
	file.open(QIODevice::ReadOnly);
	Q_ASSERT(file.isOpen() && file.isReadable());
	m_script = QString::fromUtf8(file.readAll());
	file.close();
}

void Interpreter::run(QLocalSocket* socket)
{
	socket->write("PLAY\n");
	setDevice(socket);

	writeStartDocument();
	writeStartElement("hooq");

	m_engine->evaluate(m_script, m_scriptPath);

	writeEndElement(); // hooq
	writeEndDocument();
}

void Interpreter::writeSleep(int msec)
{
	writeTextElement("msec", QString::number(msec));
}
