/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>
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

#include "ObjectInformation.h"
#include "RemoteApplicationPrototype.h"
#include "RemoteObjectPrototype.h"
#include "ScriptInterface.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QKeySequence>
#include <QTcpSocket>
#include <QPoint>
#include <QScriptContext>
#include <QScriptContextInfo>
#include <QScriptEngine>
#include <QStringList>

Interpreter::Interpreter(QObject* parent)
: QObject(parent)
, m_dumpedObject(0)
, m_pendingAcks(0)
, m_engine(new QScriptEngine(this))
{
	m_haveRequiredQtScriptExtensions = importExtension("qt.core") && importExtension("qt.gui");

	m_engine->globalObject().setProperty(
		"msleep",
		m_engine->newFunction(ScriptInterface::scriptMsleep, 1)
	);
	m_engine->globalObject().setProperty(
		"objectFromPath",
		m_engine->newFunction(ScriptInterface::scriptObjectFromPath, 1)
	);
	m_engine->globalObject().setProperty(
		"assert",
		m_engine->newFunction(ScriptInterface::scriptAssert, 1)
	);
	m_engine->globalObject().setProperty(
		"compare",
		m_engine->newFunction(ScriptInterface::scriptCompare, 2)
	);
	m_engine->globalObject().setProperty(
		"importExtension",
		m_engine->newFunction(ScriptInterface::scriptImportExtension, 1)
	);
	m_engine->globalObject().setProperty(
		"requireHooqScriptVersion",
		m_engine->newFunction(ScriptInterface::scriptRequireHooqScriptVersion, 1)
	);
	m_engine->globalObject().setProperty(
		"spawnDefaultsAndAttach",
		m_engine->newFunction(ScriptInterface::scriptSpawnDefaultsAndAttach, 0)
	);

	connect(
		ScriptInterface::instance(),
		SIGNAL(msleep(int)),
		SLOT(writeSleep(int))
	);

	connect(
		ScriptInterface::instance(),
		SIGNAL(newRemoteObject(RemoteObjectPrototype*)),
		SLOT(connectRemoteObject(RemoteObjectPrototype*))
	);

	connect(
		ScriptInterface::instance(),
		SIGNAL(newRemoteApplication(RemoteApplicationPrototype*)),
		SLOT(connectRemoteApplication(RemoteApplicationPrototype*))
	);
}

bool Interpreter::haveRequiredQtScriptExtensions() const
{
	return m_haveRequiredQtScriptExtensions;
}

QScriptEngine* Interpreter::engine() const
{
	return m_engine;
}

Interpreter::~Interpreter()
{
	delete m_dumpedObject;
}

void Interpreter::fetchProperty(const QString& path, const QString& name, QVariant* value)
{
	writeStartElement("dump");
	writeAttribute("target", path);
	writeEndElement();
	waitForDumpedObject();
	*value = m_dumpedObject->properties().value(name);
}

void Interpreter::connectRemoteApplication(RemoteApplicationPrototype* object)
{
	if(object->useDefaults())
	{
		emit startApplicationAndAttach();
	}
	else
	{
		emit startApplicationAndAttach(object->path(), object->arguments());
	}
	connect(
		object,
		SIGNAL(closeRequested()),
		SLOT(closeApplication())
	);
	waitForAttach();
}

void Interpreter::closeApplication()
{
	if(device())
	{
		writeEndElement(); // hooq
		writeEndDocument();

		waitForAck();
		device()->write("DIE\n");
		waitForAck();
		m_attachState = NotAttached;
		setDevice(0);
	}
}

void Interpreter::waitForAttach()
{
	if(m_attachState == NotAttached)
	{
		m_attachState = WaitingForAttach;
	}
	while(m_attachState != Attached)
	{
		QApplication::processEvents(QEventLoop::WaitForMoreEvents);
	}
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
		SIGNAL(mouseDoubleClickEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers)),
		SLOT(writeMouseDoubleClickEvent(QString, QPoint, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers))
	);
	connect(
		object,
		SIGNAL(contextMenuEvent(QString, QPoint, QPoint, Qt::KeyboardModifiers)),
		SLOT(writeContextMenuEvent(QString, QPoint, QPoint, Qt::KeyboardModifiers))
	);
	connect(
		object,
		SIGNAL(wheelEvent(QString, QPoint, int, Qt::MouseButtons, Qt::KeyboardModifiers, Qt::Orientation)),
		SLOT(writeWheelEvent(QString, QPoint, int, Qt::MouseButtons, Qt::KeyboardModifiers, Qt::Orientation))
	);
	connect(
		object,
		SIGNAL(closeEvent(QString)),
		SLOT(writeCloseEvent(QString))
	);
	connect(
		object,
		SIGNAL(setFocusEvent(QString, Qt::FocusReason)),
		SLOT(writeSetFocusEvent(QString, Qt::FocusReason))
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
	connect(
		object,
		SIGNAL(dragAndDropEvent(QString,QPoint,QString,QPoint)),
		SLOT(writeDragAndDropEvent(QString,QPoint,QString,QPoint))
	);
	connect(
		object,
		SIGNAL(propertyRequested(QString, QString, QVariant*)),
		SLOT(fetchProperty(QString, QString, QVariant*))
	);
	connect(
		object,
		SIGNAL(sleepRequested(int)),
		SLOT(writeSleep(int))
	);
}

bool Interpreter::ack() const
{
	return m_pendingAcks == 0;
}

void Interpreter::setAck(bool ack)
{
	if(ack)
	{
		--m_pendingAcks;
	}
	else
	{
		++m_pendingAcks;
	}
	Q_ASSERT(m_pendingAcks >= 0);
}

int Interpreter::lineNumber(QScriptContext* bottomContext)
{
	// The first valid line number in a backtrace
	QScriptContext* context = bottomContext;
	while(context)
	{
		QScriptContextInfo info(context);
		if(info.lineNumber() != -1)
		{
			return info.lineNumber();
		}
		context = context->parentContext();
	}
	return -1;
}

bool Interpreter::waitForAck()
{
	setAck(false);
	QTcpSocket* socket = static_cast<QTcpSocket*>(device());
	while(socket->state() == QTcpSocket::ConnectedState && !ack())
	{
		QApplication::processEvents(QEventLoop::WaitForMoreEvents);
	}
	if(!ack())
	{
		// We have an error
		const int line = lineNumber(m_engine->currentContext());
		m_engine->abortEvaluation();
		if(line != -1)
		{
			qDebug() << "Missing ACK at script line" << line;
			emit executionFailed(line);
		}
		else
		{
			qDebug() << "Missing ACK, couldn't find location in script";
		}
		return false;
	}
	return true;
}

void Interpreter::ensureAttached()
{
	switch(m_attachState)
	{
		case NotAttached:
			emit startApplicationAndAttach();
		case WaitingForAttach:
			waitForAttach();
			break;
		case Attached:
			return;
	}
	Q_ASSERT(m_attachState == Attached);
	Q_ASSERT(device());
}

void Interpreter::waitForDumpedObject()
{
	delete m_dumpedObject;
	m_dumpedObject = 0;
	while(!m_dumpedObject)
	{
		QApplication::processEvents();
	}
}

void Interpreter::pickObject()
{
	writeStartElement("pick");
	writeEndElement();
}

void Interpreter::writeKeyPressEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count)
{
	ensureAttached();
	writeStartElement("keyPress");
	writeKeyAttributes(path, key, modifiers, text, autorepeat, count);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeSetFocusEvent(const QString& path, Qt::FocusReason reason)
{
	ensureAttached();
	writeStartElement("focusChanged");
	writeAttribute("target", path);
	writeAttribute("reason", QString::number(reason));
	writeEndElement();
	waitForAck();
}

void Interpreter::writeCloseEvent(const QString& path)
{
	ensureAttached();
	writeStartElement("windowClosed");
	writeAttribute("target", path);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeKeyReleaseEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count)
{
	ensureAttached();
	writeStartElement("keyRelease");
	writeKeyAttributes(path, key, modifiers, text, autorepeat, count);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeMouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	ensureAttached();
	writeStartElement("mouseMove");
	writeMouseAttributes(path, position, button, buttons, modifiers);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeMousePressEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	ensureAttached();
	writeStartElement("mouseButtonPress");
	writeMouseAttributes(path, position, button, buttons, modifiers);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeMouseReleaseEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	ensureAttached();
	writeStartElement("mouseButtonRelease");
	writeMouseAttributes(path, position, button, buttons, modifiers);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeMouseDoubleClickEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	ensureAttached();
	writeStartElement("mouseButtonDoubleClick");
	writeMouseAttributes(path, position, button, buttons, modifiers);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeContextMenuEvent(const QString& path, const QPoint& position, const QPoint& globalPosition, Qt::KeyboardModifiers modifiers)
{
	ensureAttached();
	writeStartElement("contextMenu");
	writeAttribute("x", QString::number(position.x()));
	writeAttribute("y", QString::number(position.y()));
	writeAttribute("globalX", QString::number(globalPosition.x()));
	writeAttribute("globalY", QString::number(globalPosition.y()));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("target", path);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeMouseAttributes(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
{
	ensureAttached();
	writeAttribute("x", QString::number(position.x()));
	writeAttribute("y", QString::number(position.y()));
	writeAttribute("button", QString::number(button));
	writeAttribute("buttons", QString::number(buttons));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("target", path);
}

void Interpreter::writeKeyAttributes(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count)
{
	ensureAttached();
	writeAttribute("key", QString::number(key));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("text", text);
	writeAttribute("autorepeat", autorepeat ? "true" : "false");
	writeAttribute("count", QString::number(count));
	writeAttribute("target", path);
}

void Interpreter::writeShortcutEvent(const QString& path, const QKeySequence& sequence, int id, bool ambiguous)
{
	ensureAttached();
	writeStartElement("shortcut");
	writeAttribute("string", sequence.toString());
	writeAttribute("id", QString::number(id));
	writeAttribute("ambiguous", ambiguous ? "true" : "false");
	writeAttribute("target", path);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeWheelEvent(const QString& path, const QPoint& position, int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::Orientation orientation)
{
	ensureAttached();
	writeStartElement("mouseWheel");
	writeAttribute("x", QString::number(position.x()));
	writeAttribute("y", QString::number(position.y()));
	writeAttribute("delta", QString::number(delta));
	writeAttribute("buttons", QString::number(buttons));
	writeAttribute("modifiers", QString::number(modifiers));
	writeAttribute("orientation", QString::number(orientation));
	writeAttribute("target", path);
	writeEndElement();
	waitForAck();
}

void Interpreter::writeDragAndDropEvent(const QString& sourcePath, const QPoint& sourcePoint, const QString& targetPath, const QPoint& targetPoint)
{
	ensureAttached();
	writeStartElement("dragAndDrop");
	writeAttribute("source", sourcePath);
	writeAttribute("sourceX", QString::number(sourcePoint.x()));
	writeAttribute("sourceY", QString::number(sourcePoint.y()));
	writeAttribute("target", targetPath);
	writeAttribute("targetX", QString::number(targetPoint.x()));
	writeAttribute("targetY", QString::number(targetPoint.y()));
	writeEndElement();
	waitForAck();
}

void Interpreter::setScriptPath(const QString& scriptPath)
{
	QFile file(scriptPath);
	file.open(QIODevice::ReadOnly);
	Q_ASSERT(file.isOpen() && file.isReadable());
	QFileInfo info(file);
	m_scriptPath = QString("%1/%2").arg(info.dir().dirName()).arg(info.fileName());
	m_script = QString::fromUtf8(file.readAll());
	file.close();
}

void Interpreter::processSocketData()
{
	while(device()->canReadLine())
	{
		const QString line = QString::fromUtf8(device()->readLine()).trimmed();
		if(line == "ACK")
		{
			setAck();
			continue;
		}
		if(line == "PICKED")
		{
			ObjectInformation information = ObjectInformation::fromXml(device());
			qDebug() << Q_FUNC_INFO << __LINE__ << "Picked object with path" << information.path();
			emit objectPicked(information);
			continue;
		}
		if(line == "DUMPED")
		{
			m_dumpedObject = new ObjectInformation(ObjectInformation::fromXml(device()));
			continue;
		}
		if(line.startsWith("NOTFOUND"))
		{
			emit objectNotFound(line.mid(line.indexOf(' ') + 1));
			continue;
		}
		qDebug() << Q_FUNC_INFO << "Unknown response:" << line;
	}
}

void Interpreter::run()
{
	qDebug() << Q_FUNC_INFO;
	m_attachState = NotAttached;
	m_engine->evaluate(m_script, m_scriptPath);
	closeApplication();
	emit finished();
}

void Interpreter::setSocket(QTcpSocket* socket)
{
	connect(
		socket,
		SIGNAL(readyRead()),
		SLOT(processSocketData())
	);
	m_pendingAcks = 0;
	socket->write("PLAY\n");
	setDevice(socket);

	writeStartDocument();
	writeStartElement("hooq");

	m_attachState = Attached;
}

bool Interpreter::importExtension(const QString& extension)
{
	if(m_engine->importExtension(extension).isError())
	{
		qDebug() << qPrintable(QString("Failed to load %1 QtScript plugin").arg(extension));
		return false;
	}
	else
	{
		qDebug() << qPrintable(QString("Loaded %1 QtScript plugin").arg(extension));
		return true;
	}
}

void Interpreter::writeSleep(int msec)
{
	ensureAttached();
	writeTextElement("msec", QString::number(msec));
	waitForAck();
}
