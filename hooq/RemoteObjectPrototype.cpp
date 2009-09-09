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
#include "RemoteObjectPrototype.h"

#include <QDebug>
#include <QKeySequence>
#include <QPoint>

Q_DECLARE_METATYPE(Qt::FocusReason);
Q_DECLARE_METATYPE(Qt::Key);
Q_DECLARE_METATYPE(Qt::MouseButton);
Q_DECLARE_METATYPE(Qt::MouseButtons);
Q_DECLARE_METATYPE(Qt::Orientation);
Q_DECLARE_METATYPE(Qt::KeyboardModifier);

RemoteObjectPrototype::RemoteObjectPrototype(const QString& path, QObject* parent)
: QObject(parent)
, m_path(path)
{
}

RemoteObjectPrototype::~RemoteObjectPrototype()
{
}

QVariant RemoteObjectPrototype::property(const QString& name)
{
	QVariant value;
	emit propertyRequested(path(), name, &value);
	return value;
}

QString RemoteObjectPrototype::path() const
{
	return m_path;
}

void RemoteObjectPrototype::raiseMouseEvent(const QVariantMap& parameters, MouseSignal signal)
{
	emit (this->*signal)(
		path(),
		QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		),
		parameters.value("button").value<Qt::MouseButton>(),
		parameters.value("buttons").value<Qt::MouseButtons>(),
		Qt::KeyboardModifiers(static_cast<int>(parameters.value("modifiers").value<Qt::KeyboardModifier>()))
	);
}

void RemoteObjectPrototype::doubleClickMouseButton(const QVariantMap& parameters)
{
	raiseMouseEvent(parameters, &RemoteObjectPrototype::mouseDoubleClickEvent);
}

void RemoteObjectPrototype::moveMouse(const QVariantMap& parameters)
{
	raiseMouseEvent(parameters, &RemoteObjectPrototype::mouseMoveEvent);
}

void RemoteObjectPrototype::pressMouseButton(const QVariantMap& parameters)
{
	raiseMouseEvent(parameters, &RemoteObjectPrototype::mousePressEvent);
}

void RemoteObjectPrototype::releaseMouseButton(const QVariantMap& parameters)
{
	raiseMouseEvent(parameters, &RemoteObjectPrototype::mouseReleaseEvent);
}

void RemoteObjectPrototype::contextMenu(const QVariantMap& parameters)
{
	emit contextMenuEvent(
		path(),
		QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		),
		QPoint(
			parameters.value("globalX").toInt(),
			parameters.value("globalY").toInt()
		),
		Qt::KeyboardModifiers(static_cast<int>(parameters.value("modifiers").value<Qt::KeyboardModifier>()))
	);
}

void RemoteObjectPrototype::raiseKeyEvent(const QVariantMap& parameters, KeySignal signal)
{
	emit (this->*signal)(
		path(),
		static_cast<int>(parameters.value("key").value<Qt::Key>()),
		Qt::KeyboardModifiers(static_cast<int>(parameters.value("modifiers").value<Qt::KeyboardModifier>())),
		parameters.value("text").toString(),
		parameters.value("autorepeat").toBool(),
		parameters.value("count").value<ushort>()
	);
}

void RemoteObjectPrototype::pressKey(const QVariantMap& parameters)
{
	raiseKeyEvent(parameters, &RemoteObjectPrototype::keyPressEvent);
}

void RemoteObjectPrototype::setFocus(const QVariantMap& parameters)
{
	emit setFocusEvent(
		path(),
		parameters.value("reason").value<Qt::FocusReason>()
	);
}

void RemoteObjectPrototype::releaseKey(const QVariantMap& parameters)
{
	raiseKeyEvent(parameters, &RemoteObjectPrototype::keyReleaseEvent);
}

void RemoteObjectPrototype::shortcut(const QVariantMap& parameters)
{
	emit shortcutEvent(
		path(),
		QKeySequence::fromString(parameters.value("string").toString()),
		parameters.value("id").toInt(),
		parameters.value("ambiguous").toBool()
	);
}

void RemoteObjectPrototype::mouseWheel(const QVariantMap& parameters)
{
	emit wheelEvent(
		path(),
		QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		),
		parameters.value("delta").toInt(),
		parameters.value("buttons").value<Qt::MouseButtons>(),
		Qt::KeyboardModifiers(static_cast<int>(parameters.value("modifiers").value<Qt::KeyboardModifier>())),
		parameters.value("orientation").value<Qt::Orientation>()
	);
}
