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
#include "Player.h"

#include "Event.h"
#include "XmlPropertyDumper.h"

#include "../common/ObjectHookName.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QShortcutEvent>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QWidget>

namespace Hooq
{

bool Player::hook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	// returnType* returnValue = reinterpret_cast<returnType*>(data[2])
	return instance()->hook(receiver, event);
}

bool Player::hook(QObject* receiver, QEvent* event)
{
	if(event->type() == QEvent::MouseButtonPress)
	{
		return true;
	}
	if(event->type() == QEvent::MouseButtonRelease)
	{
		Q_ASSERT(device()->isWritable());
		device()->write("PICKED\n");
		XmlPropertyDumper::dump(receiver, device());
		endPick();
		return true;
	}
	return false;
}

void Player::startPick()
{
	m_pickWidget->show();
	m_pickWidget->raise();
	// Crosshair
	QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
	// Start listening for events
	QInternal::registerCallback(QInternal::EventNotifyCallback, &hook);
}

void Player::endPick()
{
	m_pickWidget->hide();
	// Remove our crosshair
	QApplication::restoreOverrideCursor();
	// Remove our hook
	QInternal::unregisterCallback(QInternal::EventNotifyCallback, &hook);
	// Continue with our queue
	m_processingEvents = false;
	processEvents();
}

QPointer<Player> Player::m_instance;

Player* Player::instance(QIODevice* device)
{
	m_instance = QPointer<Player>(new Player(device));
	return m_instance.data();
}

Player* Player::instance()
{
	return m_instance.data();
}

void Player::sleep(int msec)
{
	QTimer::singleShot(msec, this, SLOT(waitFinished()));
}

void Player::waitFinished()
{
	ack();
	m_processingEvents = false;
	processEvents();
}

void Player::ack()
{
	device()->write("ACK\n");
}

Player::Player(QIODevice* device)
: QObject()
, m_processingEvents(false)
{
	disconnect(device, 0, 0, 0);
	m_pickWidget = new QLabel(tr("Click on a widget to retrieve its properties."));

	setDevice(device);
	connect(
		device,
		SIGNAL(readyRead()),
		SLOT(readNext())
	);
	readNext();
}

void Player::readNext()
{
	while(!atEnd())
	{
		QXmlStreamReader::readNext();
		if(tokenType() == StartElement)
		{
			handleElement();
			processEvents();
		}
		if(tokenType() == EndDocument)
		{
			disconnect(device(), 0, this, 0);
			ack();
			setDevice(0);
			deleteLater();
			emit finished();
			return;
		}
	}
	processEvents();
}

void Player::processEvents()
{
	if(m_processingEvents)
	{
		return;
	}
	m_processingEvents = true;
	while(!m_eventQueue.isEmpty())
	{
		Event* event = m_eventQueue.dequeue();
		switch(event->type())
		{
			case Event::Dump:
			{
				DumpEvent* e = static_cast<DumpEvent*>(event);
				QObject* o = findObject(e->objectPath());
				Q_ASSERT(o);
				device()->write("DUMPED\n");
				XmlPropertyDumper::dump(o, device());
				break;
			};
			case Event::Object:
			{
				ObjectEvent* o = static_cast<ObjectEvent*>(event);
				QObject* receiver = findObject(o->objectPath());
				if(!receiver)
				{
					qDebug() << "Couldn't find receiver" << receiver;
					continue;
				}
				QCoreApplication::postEvent(receiver, o->qtEvent());
				ack();
				break;
			}
			case Event::Pick:
				startPick();
				return;
			case Event::Sleep:
				sleep(static_cast<SleepEvent*>(event)->msec());
				delete event;
				return;
		}
	}
	m_processingEvents = false;
}

void Player::postPickEvent()
{
	m_eventQueue.enqueue(new PickEvent());
}

void Player::postSleepEvent()
{
	m_eventQueue.enqueue(new SleepEvent(readElementText().toInt()));
}

void Player::handleElement()
{
	qDebug() << Q_FUNC_INFO << name();
	// "Magic" events
	if(name() == "msec")
	{
		postSleepEvent();
	}
	if(name() == "dump")
	{
		postDumpEvent();
	}
	if(name() == "pick")
	{
		postPickEvent();
	}

	// QEvents
	if(name() == "keyPress")
	{
		postKeyEvent(QEvent::KeyPress);
	}
	if(name() == "keyRelease")
	{
		postKeyEvent(QEvent::KeyRelease);
	}
	if(name() == "mouseMove")
	{
		postMouseEvent(QEvent::MouseMove);
	}
	if(name() == "mouseButtonPress")
	{
		postMouseEvent(QEvent::MouseButtonPress);
	}
	if(name() == "mouseButtonDoubleClick")
	{
		postMouseEvent(QEvent::MouseButtonDblClick);
	}
	if(name() == "mouseButtonRelease")
	{
		postMouseEvent(QEvent::MouseButtonRelease);
	}
	if(name() == "mouseWheel")
	{
		postWheelEvent();
	}
	if(name() == "shortcut")
	{
		postShortcutEvent();
	}
}

void Player::postShortcutEvent()
{
	QShortcutEvent* event = new QShortcutEvent(
		QKeySequence::fromString(attributes().value("string").toString()),
		attributes().value("id").toString().toInt(),
		attributes().value("ambiguous").toString() == "true"
	);

	m_eventQueue.enqueue(new ObjectEvent(attributes().value("target").toString(), event));
}

void Player::postKeyEvent(int type)
{
	QKeyEvent* event = new QKeyEvent(
		static_cast<QEvent::Type>(type),
		attributes().value("key").toString().toInt(),
		static_cast<Qt::KeyboardModifiers>(attributes().value("modifiers").toString().toInt()),
		attributes().value("text").toString(),
		attributes().value("isAutoRepeat") == "true",
		attributes().value("count").toString().toUShort()
	);

	m_eventQueue.enqueue(new ObjectEvent(attributes().value("target").toString(), event));
}

void Player::postMouseEvent(int type)
{
	QMouseEvent* event = new QMouseEvent(
		static_cast<QEvent::Type>(type),
		QPoint(
			attributes().value("x").toString().toInt(),
			attributes().value("y").toString().toInt()
		),
		static_cast<Qt::MouseButton>(attributes().value("button").toString().toInt()),
		static_cast<Qt::MouseButtons>(attributes().value("buttons").toString().toInt()),
		static_cast<Qt::KeyboardModifiers>(attributes().value("modifiers").toString().toInt())
	);
	m_eventQueue.enqueue(new ObjectEvent(attributes().value("target").toString(), event));
}

void Player::postDumpEvent()
{
	m_eventQueue.enqueue(new DumpEvent(attributes().value("target").toString()));
}

void Player::postWheelEvent()
{
	QWheelEvent* event = new QWheelEvent(
		QPoint(
			attributes().value("x").toString().toInt(),
			attributes().value("y").toString().toInt()
		),
		attributes().value("delta").toString().toInt(),
		static_cast<Qt::MouseButtons>(attributes().value("buttons").toString().toInt()),
		static_cast<Qt::KeyboardModifiers>(attributes().value("modifiers").toString().toInt()),
		attributes().value("orientation") == "vertical" ? Qt::Vertical : Qt::Horizontal
	);

	m_eventQueue.enqueue(new ObjectEvent(attributes().value("target").toString(), event));
}

QObject* Player::findObject(const QString& path)
{
	const QString separator("::");
	QStringList parts = path.split(separator);
	if(parts.isEmpty())
	{
		return 0;
	}
	const QString name = parts.takeLast();
	QObject* parent = 0;
	if(parts.isEmpty())
	{
		// Top level widget
		Q_FOREACH(QWidget* widget, QApplication::topLevelWidgets())
		{
			if(ObjectHookName::objectName(widget) == name)
			{
				return widget;
			}
		}
		return 0;
	}
	else
	{
		parent = findObject(parts.join(separator));
		if(!parent)
		{
			return 0;
		}
	}
	Q_FOREACH(QObject* child, parent->children())
	{
		if(ObjectHookName::objectName(child) == name)
		{
			return child;
		}
	}
	
	return 0;
}

} // namespace
