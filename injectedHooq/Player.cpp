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
#include "Player.h"

#include "Event.h"
#include "XmlPropertyDumper.h"

#include "../common/ObjectHookName.h"

#include <QApplication>
#include <QContextMenuEvent>
#include <QCursor>
#include <QDebug>
#include <QFile>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QShortcutEvent>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QWidget>

#ifdef Q_OS_WIN32

#include <QDrag>
// Need to manually synthesise this
#include <QDropEvent>

#endif

#include <memory> // for std::auto_ptr - XXX FIXME XXX switch to QScopedPointer in 4.6

namespace Hooq
{

Player::~Player()
{
	// Remove our hook
	QInternal::unregisterCallback(QInternal::EventNotifyCallback, hook);
}

bool Player::hook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	// returnType* returnValue = reinterpret_cast<returnType*>(data[2])
	return instance()->eventFilter(receiver, event);
}

bool Player::eventFilter(QObject* receiver, QEvent* event)
{
	if(dynamic_cast<QDropEvent*>(event))
	{
		qDebug() << "IS A DROP EVENT";
		qDebug() << "Receiver:" << receiver;
		qDebug() << "Event type:" << event->type();
	}
	if(m_mode == Pick)
	{
		if(event->type() == QEvent::MouseButtonPress)
		{
			return true;
		}
		if(event->type() == QEvent::MouseButtonRelease)
		{
			Q_ASSERT(device()->isWritable());
			device()->write("PICKED\n");
			QWidget* widget = qobject_cast<QWidget*>(receiver);
			if(widget && widget->focusProxy())
			{
				XmlPropertyDumper::dump(widget->focusProxy(), device());
			}
			else
			{
				XmlPropertyDumper::dump(receiver, device());
			}
			endPick();
			return true;
		}
	}
	return false;
}

void Player::startPick()
{
	m_mode = Pick;
	m_pickWidget->show();
	m_pickWidget->raise();
	// Crosshair
	QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
}

void Player::endPick()
{
	m_mode = Playback;
	m_pickWidget->hide();
	// Remove our crosshair
	QApplication::restoreOverrideCursor();
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
	if(m_ackNext)
	{
		device()->write("ACK\n");
	}
}

void Player::notifyNotFound(PathEvent* event)
{
	device()->write("NOTFOUND ");
	device()->write(event->objectPath().toLatin1());
	device()->write("\n", 1);
}

Player::Player(QIODevice* device)
: QObject()
, m_processingEvents(false)
, m_mode(Playback)
{
	disconnect(device, 0, 0, 0);
	m_pickWidget = new QLabel(tr("Click on a widget to retrieve its properties."));

	setDevice(device);
	connect(
		device,
		SIGNAL(readyRead()),
		SLOT(readNext())
	);

	// Start listening for events
	QInternal::registerCallback(QInternal::EventNotifyCallback, hook);
}

void Player::run()
{
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
			ack();
			disconnect(device(), 0, this, 0);
			break;
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
		std::auto_ptr<Event> event(m_eventQueue.dequeue());
		if(!event->tag().isNull())
		{
			qDebug() << "Processing event" << event->tag();
		}
		m_ackNext = event->ack();
		// "break;" for ack, "continue;" for no-ack, "return;" to leave the event loop - be sure to re-enter it later
		switch(event->type())
		{
			case Event::Ack:
				// no-op
				break;
			case Event::Dump:
			{
				DumpEvent* e = Hooq::event_cast<DumpEvent*>(event.get());
				QObject* o = findObject(e->objectPath());
				Q_ASSERT(o);
				device()->write("DUMPED\n");
				XmlPropertyDumper::dump(o, device());
				continue;
			}
			case Event::Flush:
			{
				qDebug() << "Calling processEvents";
				QApplication::processEvents();
				continue;
			}
			case Event::Focus:
			{
				FocusEvent* e = Hooq::event_cast<FocusEvent*>(event.get());
				QObject* o = findObject(e->objectPath());
				QWidget* w = qobject_cast<QWidget*>(o);
				if(w)
				{
					w->setFocus(e->reason());
				}
				else
				{
					qDebug() << "Couldn't find focus widget from" << e->objectPath() << "- QObject:" << o;
					notifyNotFound(e);
				}
				break;
			}
			case Event::Close:
			{
				CloseEvent* e = Hooq::event_cast<CloseEvent*>(event.get());
				Q_ASSERT(e);
				QObject* o = findObject(e->objectPath());
				QWidget* w = qobject_cast<QWidget*>(o);
				if(w)
				{
					w->close();
				}
				else
				{
					qDebug() << "Couldn't find widget to close from" << e->objectPath() << "- QObject:" << o;
					Q_ASSERT(o);
					Q_ASSERT(w);
				}
				break;
			}
			case Event::Drop:
			{
				DropEvent* e = Hooq::event_cast<DropEvent*>(event.get());
				QObject* o = findObject(e->objectPath());
				if(o)
				{
					// Windows: grab the internal data to do the drop
#ifdef Q_OS_WIN32
#endif
					/*
					QDropEvent de(
						e->pos(),
						Qt::ActionMask,
						QDragManager::self()->object->mimeData(),
						Qt::NoButton,
						Qt::NoModifier
					);
					qApp->sendEvent(o, &de);
					*/
					// X11: Actual drag
					// Windows: avoid a press without release
					QMouseEvent* me = new QMouseEvent(
						QEvent::MouseButtonRelease,
						e->pos(),
						e->globalPos(),
						Qt::LeftButton,
						Qt::NoButton,
						Qt::NoModifier
					);
					qApp->postEvent(o, me);
				}
				else
				{
					qDebug() << "Couldn't find receiver from path" << e->objectPath();
					notifyNotFound(e);
				}
				break;
			}
			case Event::Object:
			{
				ObjectEvent* e = Hooq::event_cast<ObjectEvent*>(event.get());
				QObject* o = findObject(e->objectPath());
				if(o)
				{
					if(e->qtEvent()->type() == QEvent::MouseButtonPress)
					{
						QMouseEvent* m = static_cast<QMouseEvent*>(e->qtEvent());
						qDebug() << "Posting button press to" << o << m->pos() << m->button() << m->buttons();
					}
					if(e->qtEvent()->type() == QEvent::MouseMove)
					{
						QMouseEvent* m = static_cast<QMouseEvent*>(e->qtEvent());
						qDebug() << "Posting move event to" << o << m->pos() << m->button() << m->buttons();
					}
					qApp->postEvent(o, e->qtEvent());
				}
				else
				{
					qDebug() << "Couldn't find receiver from path" << e->objectPath();
					notifyNotFound(e);
				}
				break;
			}
			case Event::Pick:
				startPick();
				return;
			case Event::Sleep:
				sleep(Hooq::event_cast<SleepEvent*>(event.get())->msec());
				return;
		}
		ack();
	}

	if(tokenType() == EndDocument)
	{
		setDevice(0);
		emit finished();
	}
	m_processingEvents = false;
}

void Player::debugPrintObjectTree(int nestingDepth, QWidget* root, TextStreamPointer streamPointer)
{
	QFile file;
	if(!streamPointer)
	{
		file.setFileName("/tmp/hooq-object-tree");
		qDebug() << "Dumping object tree to" << file.fileName();
		file.open(QIODevice::WriteOnly | QFile::Truncate);
		streamPointer = TextStreamPointer(new QTextStream(&file));
	}
	QTextStream& stream = *streamPointer;

	QList<QWidget*> children;
	if(root == 0)
	{
		stream << "****************************" << endl;
		stream << "***** WIDGET TREE DUMP *****" << endl;
		stream << "****************************" << endl;
		children = QApplication::topLevelWidgets();
	}
	else
	{
		Q_FOREACH(QObject* object, root->children())
		{
			QWidget* widget = qobject_cast<QWidget*>(object);
			if(widget)
			{
				children.append(widget);
			}
		}
	}

	Q_FOREACH(QWidget* widget, children)
	{
		stream << QString(nestingDepth * 4, '.') << ObjectHookName::objectName(widget) << endl; 
		debugPrintObjectTree(nestingDepth + 1, widget, streamPointer);
	}
}

void Player::postPickEvent()
{
	m_eventQueue.enqueue(new PickEvent());
}

void Player::postSleepEvent()
{
	m_eventQueue.enqueue(new SleepEvent(readElementText().toInt()));
}

void Player::postDragAndDrop()
{
	// Make sure that any events you queue up have setAck(false) called, except for the last, otherwise the GUI gets sent multiple ACKs for one command
	const QString sourcePath = attributes().value("source").toString();
	const QPoint sourcePoint = QPoint(attributes().value("sourceX").toString().toInt(), attributes().value("sourceY").toString().toInt());
	const QString targetPath = attributes().value("target").toString();
	const QPoint targetPoint = QPoint(attributes().value("targetX").toString().toInt(), attributes().value("targetY").toString().toInt());
	readElementText();
	qDebug() << "Read drag and drop event" << sourcePath << sourcePoint << targetPath << targetPoint;
	// These explicit flushes are an alternative to zero-second sleeps
#ifndef Q_OS_WIN32
	m_eventQueue.enqueue(Event::withoutAck(new FlushEvent()));
#endif
	// 1. Start the drag
	// 1.1 Give focus
	m_eventQueue.enqueue(
		Event::addTag("dnd_sourceFocus", Event::withoutAck(
			new FocusEvent(
				sourcePath,
				Qt::OtherFocusReason
			)
		))
	);
	// 1.2. Click
#ifndef Q_OS_WIN32
	m_eventQueue.enqueue(Event::withoutAck(new FlushEvent()));
#endif
	m_eventQueue.enqueue(
		Event::addTag("dnd_click", Event::withoutAck(
			new ObjectEvent(
				sourcePath,
				new QMouseEvent(
					QEvent::MouseButtonPress,
					sourcePoint,
					Qt::LeftButton,
					Qt::LeftButton,
					Qt::NoModifier
				)
			)
		))
	);
	// 1.3. Give it a little nudge (QAbstractItemView requires two passes through mouseMoveEvent to start the drag)
	m_eventQueue.enqueue(
		Event::addTag("dnd_nudge", Event::withoutAck(
			new ObjectEvent(
				sourcePath,
				new QMouseEvent(
					QEvent::MouseMove,
					sourcePoint + QPoint(0, 1),
					Qt::NoButton,
					Qt::LeftButton,
					Qt::NoModifier
				)
			)
		))
	);
	// 1.3. (reprise) Wait for drag time interval
	m_eventQueue.enqueue(Event::addTag("dnd_waitBeforeDrag", Event::withoutAck(new SleepEvent(qApp->startDragTime() + 1)))); // +1 here is just in case someone else has a similar issue to the QAbstractItemView one below
	// 1.4. Drag it for the minimum distance
	m_eventQueue.enqueue(
		Event::addTag("dnd_initialDrag", Event::withoutAck(
			new ObjectEvent(
				sourcePath,
				new QMouseEvent(
					QEvent::MouseMove,
					sourcePoint + QPoint(0, qApp->startDragDistance() + 1), // QAbstractItemView needs the +1, at least in 4.6.0
					Qt::NoButton,
					Qt::LeftButton,
					Qt::NoModifier
				)
			)
		))
	);
	// 2. Now we've started the drag (hopefully), drop it
	// DO NOT INSERT ANY MORE FLUSHES; once the drag starts, at least on X11, we don't get another
	// chance to put stuff onto the event queue.
	///@todo Check that a drag has actually started
	// 2.1. Move the mouse first, just to be friendly
	m_eventQueue.enqueue(
		Event::addTag("dnd_mainDrag", Event::withoutAck(
			new ObjectEvent(
				targetPath,
				new QMouseEvent(
					QEvent::MouseMove,
					targetPoint,
					qobject_cast<QWidget*>(findObject(targetPath))->mapToGlobal(targetPoint),
					Qt::NoButton,
					Qt::LeftButton,
					Qt::NoModifier
				)
			)
		))
	);
	// 2.2. Now, release the hounds^Wmouse button
	m_eventQueue.enqueue(
		Event::addTag("dnd_drop", Event::withoutAck(
			new DropEvent(
				targetPath,
				targetPoint,
				qobject_cast<QWidget*>(findObject(targetPath))->mapToGlobal(targetPoint)
			)
		))
	);

	// ACK the DnD as a whole
	m_eventQueue.enqueue(Event::withoutAck(new FlushEvent()));
	m_eventQueue.enqueue(new AckEvent());
}

void Player::handleElement()
{
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
	if(name() == "dragAndDrop")
	{
		postDragAndDrop();
	}

	// QEvents
	if(name() == "windowClosed")
	{
		postCloseEvent();
	}
	if(name() == "contextMenu")
	{
		postContextMenuEvent();
	}
	if(name() == "focusChanged")
	{
		postFocusEvent();
	}
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

void Player::postCloseEvent()
{
	m_eventQueue.enqueue(new CloseEvent(attributes().value("target").toString()));
}

void Player::postContextMenuEvent()
{
	QContextMenuEvent* event = new QContextMenuEvent(
		static_cast<QContextMenuEvent::Reason>(attributes().value("reason").toString().toInt()),
		QPoint(
			attributes().value("x").toString().toInt(),
			attributes().value("y").toString().toInt()
		),
		QPoint(
			attributes().value("globalX").toString().toInt(),
			attributes().value("globalY").toString().toInt()
		),
		static_cast<Qt::KeyboardModifiers>(attributes().value("modifiers").toString().toInt())
	);
	
	m_eventQueue.enqueue(new ObjectEvent(attributes().value("target").toString(), event));
}

void Player::postFocusEvent()
{
	const Qt::FocusReason reason = static_cast<Qt::FocusReason>(attributes().value("reason").toString().toInt());
	const QString target = attributes().value("target").toString();
	m_eventQueue.enqueue(new FocusEvent(target, reason));
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
