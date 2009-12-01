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
#include "Logger.h"

#include "CanaryEvent.h"

#include "../common/Communication.h"
#include "../common/ObjectHookName.h"

#include <Qt>

#include <QCoreApplication>
#include <QContextMenuEvent>
#include <QDebug>
#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QObject>
#include <QShortcutEvent>
#include <QTextStream>
#include <QTime>
#include <QWheelEvent>
#include <QWidget>

namespace Hooq
{

QPointer<Logger> Logger::m_instance;

Logger* Logger::instance(QIODevice* device)
{
	m_instance = QPointer<Logger>(new Logger(device));
	return m_instance.data();
}

Logger* Logger::instance()
{
	return m_instance.data();
}

Logger::Logger(QIODevice* device)
: QObject()
, m_ignoreEvents(false)
{
	disconnect(device, 0, 0, 0);

	m_writer.setDevice(device);
	m_writer.setAutoFormatting(true);
	m_writer.writeStartDocument();
	m_writer.writeStartElement("hooq");
	m_timer.start();
	activate();
	connect(
		device,
		SIGNAL(readyRead()),
		SLOT(readInput())
	);
}

void Logger::readInput()
{
	if(m_writer.device()->readLine().constData() == QLatin1String("STOP"))
	{
		emit finished();
		deleteLater();
	}
}

Logger::~Logger()
{
	m_writer.writeEndDocument();
}

void Logger::activate()
{
	qAddPostRoutine(deactivate);
	// Internal API... seems unavoidable
	QInternal::registerCallback(QInternal::EventNotifyCallback, &hook);
}

void Logger::deactivate()
{
	delete instance();
	QInternal::unregisterCallback(QInternal::EventNotifyCallback, &hook);
}

bool Logger::hook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	// returnType* returnValue = reinterpret_cast<returnType*>(data[2])
	return instance()->eventFilter(receiver, event);
}

QObject* Logger::focusObject(QObject* receiver)
{
	QWidget* widget = qobject_cast<QWidget*>(receiver);
	if(!widget)
	{
		return 0;
	}

	if(widget->focusProxy())
	{
		return widget->focusProxy();
	}
	return widget;
}

bool Logger::eventFilter(QObject* receiver, QEvent* event)
{
	// Check that we don't receive a canary when we've not put one in the queue
	Q_ASSERT(event->type() != CanaryEvent::staticType() || m_ignoreEvents);

	if(m_ignoreEvents)
	{
		return false;
	}

	switch(event->type())
	{
		case QEvent::ContextMenu:
			outputEvent(receiver, "contextMenu", contextMenuEventAttributes(static_cast<QContextMenuEvent*>(event)));
			break;
		case QEvent::FocusIn:
			outputEvent(focusObject(receiver), "focusChanged", focusEventAttributes(static_cast<QFocusEvent*>(event)), receiver);
			break;
		case QEvent::KeyPress:
			outputEvent(receiver, "keyPress", keyEventAttributes(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::KeyRelease:
			outputEvent(receiver, "keyRelease", keyEventAttributes(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::MouseMove:
			outputEvent(receiver, "mouseMove", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonPress:
			outputEvent(receiver, "mouseButtonPress", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonRelease:
			outputEvent(receiver, "mouseButtonRelease", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonDblClick:
			outputEvent(receiver, "mouseButtonDoubleClick", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::Shortcut:
			outputEvent(receiver, "shortcut", shortcutEventAttributes(static_cast<QShortcutEvent*>(event)));
			break;
		case QEvent::Wheel:
			outputEvent(receiver, "mouseWheel", wheelEventAttributes(static_cast<QWheelEvent*>(event)));
			break;
		default:
			break;

	}
	QCoreApplication::postEvent(this, new CanaryEvent(&m_ignoreEvents));
	return false;
}

void Logger::outputEvent(QObject* receiver, const char* event, const QXmlStreamAttributes& attributes, QObject* originalReceiver)
{
	if(!receiver)
	{
		// ::focusObject() returns NULL for non-widgets; for example, the KDE oxygen style
		// receives focusIn/focusOut events
		return;
	}

	m_writer.writeTextElement("msec", QString::number(m_timer.restart()));
	
	m_writer.writeStartElement(event);
	m_writer.writeAttributes(attributes);
	m_writer.writeAttribute("target", ObjectHookName::objectPath(receiver));
	m_writer.writeAttribute("targetClass", receiver->metaObject()->className());
	if(originalReceiver && receiver != originalReceiver)
	{
		m_writer.writeAttribute("originalTarget", ObjectHookName::objectPath(originalReceiver));
	}
	m_writer.writeEndElement(); //event;
}

QString Logger::safeText(const QString& string)
{
	QString out;
	Q_FOREACH(const QChar& character, string)
	{
		if(character.isPrint())
		{
			out.append(character);
		}
	}
	return out;
}

QXmlStreamAttributes Logger::contextMenuEventAttributes(QContextMenuEvent* event)
{
	QXmlStreamAttributes data;
	data.append("x", QString::number(event->x()));
	data.append("y", QString::number(event->y()));
	data.append("globalX", QString::number(event->globalX()));
	data.append("globalY", QString::number(event->globalY()));
	data.append("reason", QString::number(event->reason()));
	data.append("modifiers", QString::number(event->modifiers())); // Qt::Modifiers
	return data;
}

QXmlStreamAttributes Logger::keyEventAttributes(QKeyEvent* event)
{
	QXmlStreamAttributes data;
	data.append("key", QString::number(event->key())); // Qt::Key
	data.append("modifiers", QString::number(event->modifiers())); // Qt::Modifiers
	data.append("text", safeText(event->text()));
	data.append("isAutoRepeat", event->isAutoRepeat() ? "true" : "false");
	data.append("count", QString::number(event->count()));
	return data;
}

QXmlStreamAttributes Logger::mouseEventAttributes(QMouseEvent* event)
{
	QXmlStreamAttributes data;
	data.append("x", QString::number(event->x()));
	data.append("y", QString::number(event->y()));
	data.append("button", QString::number(event->button()));
	data.append("buttons", QString::number(event->buttons()));
	data.append("modifiers", QString::number(event->modifiers())); // Qt::Modifiers
	return data;
}

QXmlStreamAttributes Logger::wheelEventAttributes(QWheelEvent* event)
{
	QXmlStreamAttributes data;
	data.append("x", QString::number(event->x()));
	data.append("y", QString::number(event->y()));
	data.append("delta", QString::number(event->delta()));
	data.append("buttons", QString::number(event->buttons()));
	data.append("modifiers", QString::number(event->modifiers())); // Qt::Modifiers
	data.append("orientation", event->orientation() == Qt::Vertical ? "vertical" : "horizontal");
	return data;
}

QXmlStreamAttributes Logger::shortcutEventAttributes(QShortcutEvent* event)
{
	QXmlStreamAttributes data;
	data.append("string", event->key().toString());
	data.append("id", QString::number(event->shortcutId()));
	data.append("ambiguous", event->isAmbiguous() ? "true" : "false");
	return data;
}

QXmlStreamAttributes Logger::focusEventAttributes(QFocusEvent* event)
{
	QXmlStreamAttributes data;
	data.append("reason", QString::number(event->reason()));
	return data;
}

} //namespace
