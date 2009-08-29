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
#pragma once

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
		void moveMouse(const QVariantMap& parameters);
		void pressMouseButton(const QVariantMap& parameters);
		void releaseMouseButton(const QVariantMap& parameters);
		void mouseWheel(const QVariantMap& parameters);
		void pressKey(const QVariantMap& parameters);
		void releaseKey(const QVariantMap& parameters);
		void shortcut(const QVariantMap& parameters);
	signals:
		void mouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void mousePressEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void mouseReleaseEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void wheelEvent(const QString& path, const QPoint& position, int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::Orientation orientation);
		void keyPressEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void keyReleaseEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void shortcutEvent(const QString& path, const QKeySequence& sequence, int id, bool ambiguous);
	private:
		const QString m_path;
};
