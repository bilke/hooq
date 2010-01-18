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
#pragma once

#include <QScriptValue>
#include <QScriptable>
#include <QVariant>

class QKeySequence;

class RemoteObjectPrototype : public QObject, public QScriptable
{
	Q_OBJECT
	public:
		RemoteObjectPrototype(const QString& path, QObject* parent = 0);
		~RemoteObjectPrototype();
		
		QString path() const;
	public slots:
		void close();
		void moveMouse(const QVariantMap& parameters);
		void pressMouseButton(const QVariantMap& parameters);
		void releaseMouseButton(const QVariantMap& parameters);
		void doubleClickMouseButton(const QVariantMap& parameters);
		void mouseWheel(const QVariantMap& parameters);
		void pressKey(const QVariantMap& parameters);
		void releaseKey(const QVariantMap& parameters);
		void shortcut(const QVariantMap& parameters);
		void contextMenu(const QVariantMap& parameters);
		void setFocus(const QVariantMap& parameters);
		QVariant property(const QString& name);
		QScriptValue drag(const QVariantMap& parameters);
		void drop(const QScriptValue& parameters);
		void sendText(const QString& text);
	signals:
		void closeEvent(const QString& path);
		void sleepRequested(int msec);
		void setFocusEvent(const QString& path, Qt::FocusReason reason);
		void propertyRequested(const QString& path, const QString& name, QVariant* result);
		void mouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void mousePressEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void mouseReleaseEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void mouseDoubleClickEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void wheelEvent(const QString& path, const QPoint& position, int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::Orientation orientation);
		void keyPressEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void keyReleaseEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void shortcutEvent(const QString& path, const QKeySequence& sequence, int id, bool ambiguous);
		void contextMenuEvent(const QString& path, const QPoint& position, const QPoint& globalPosition, Qt::KeyboardModifiers modifiers);
		void dragAndDropEvent(const QString& sourcePath, const QPoint& sourcePosition, const QString& targetPath, const QPoint& targetPosition);
	private:
		static Qt::Key keyForChar(const QChar& character);
		/// KeySignal typedef - pointer to either the keyPressEvent or keyReleaseEvent signals
		typedef void(RemoteObjectPrototype::*KeySignal)(const QString&, int, Qt::KeyboardModifiers, const QString&, bool, ushort);
		/// MouseSignal typedef - pointer to any of the mouse signals
		typedef void(RemoteObjectPrototype::*MouseSignal)(const QString&, const QPoint&, Qt::MouseButton, Qt::MouseButtons, Qt::KeyboardModifiers);

		void raiseKeyEvent(const QVariantMap& parameters, KeySignal signal);
		void raiseMouseEvent(const QVariantMap& parameters, MouseSignal signal);
		const QString m_path;
};
