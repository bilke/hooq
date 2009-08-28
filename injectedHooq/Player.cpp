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
#include "../common/ObjectHookName.h"


#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QShortcutEvent>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QWidget>

namespace Hooq
{

void Player::sleep(int msec)
{
	QTimer::singleShot(msec, this, SLOT(readNext()));
}

Player::Player(QIODevice* device)
{
	setDevice(device);
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
			return;
		}
		if(tokenType() == EndDocument)
		{
			emit finished();
			return;
		}
	}
}

void Player::handleElement()
{
	if(name() == "msec")
	{
		sleep(readElementText().toInt());
		return;
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
	readNext();
}

void Player::postShortcutEvent()
{
	QObject* object = findObject(attributes().value("target").toString());
	if(!object)
	{
		return;
	}

	QShortcutEvent* event = new QShortcutEvent(
		QKeySequence::fromString(attributes().value("string").toString()),
		attributes().value("id").toString().toInt(),
		attributes().value("ambiguous").toString() == "true"
	);

	QCoreApplication::postEvent(object, event);
}

void Player::postKeyEvent(int type)
{
	QObject* object = findObject(attributes().value("target").toString());
	if(!object)
	{
		return;
	}

	QKeyEvent* event = new QKeyEvent(
		static_cast<QEvent::Type>(type),
		attributes().value("key").toString().toInt(),
		static_cast<Qt::KeyboardModifiers>(attributes().value("modifiers").toString().toInt()),
		attributes().value("text").toString(),
		attributes().value("isAutoRepeat") == "true",
		attributes().value("count").toString().toUShort()
	);

	QCoreApplication::postEvent(object, event);
}

void Player::postMouseEvent(int type)
{
	QObject* object = findObject(attributes().value("target").toString());
	if(!object)
	{
		return;
	}

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
	QCoreApplication::postEvent(object, event);
}

void Player::postWheelEvent()
{
	QObject* object = findObject(attributes().value("target").toString());
	if(!object)
	{
		return;
	}

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
	QCoreApplication::postEvent(object, event);
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
