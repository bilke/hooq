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

#include <QObject>
#include <QPoint>
#include <QPointer>
#include <QString>
#include <QTime>
#include <QXmlStreamWriter>

class QContextMenuEvent;
class QDropEvent;
class QFocusEvent;
class QKeyEvent;
class QMouseEvent;
class QShortcutEvent;
class QWheelEvent;

namespace Hooq
{

class Logger : public QObject
{
	Q_OBJECT
	public:
		~Logger();
		static Logger* instance(QIODevice*);
	signals:
		void finished();
	private slots:
		void readInput();
	private:
		static QObject* focusObject(QObject* object);
		static Logger* instance();
		Logger(QIODevice* device);
		static QPointer<Logger> m_instance;
		static void activate();
		static void deactivate();

		/// The main hook.
		static bool hook(void** data);
		bool eventFilter(QObject* receiver, QEvent* event);

		QXmlStreamWriter m_writer;
		QTime m_timer;
		QPoint m_dragOriginPoint;
		QObject* m_dragOriginWidget;

		void outputEvent(QObject* receiver, const char* event, const QXmlStreamAttributes& attributes, QObject* originalReceiver = 0);
		void outputDragAndDropEvent(QObject* receiver, QDropEvent* event);

		static QString safeText(const QString&);

		/// Return a list of parameters for a shortcut event.
		static QXmlStreamAttributes shortcutEventAttributes(QShortcutEvent* event);
		/// Return a list of parameters for a key event.
		static QXmlStreamAttributes keyEventAttributes(QKeyEvent* event);
		/// Return a list of parameters for a mouse press/release/move event.
		static QXmlStreamAttributes mouseEventAttributes(QMouseEvent* event);
		/// Return a list of parameters for a mouse wheel event
		static QXmlStreamAttributes wheelEventAttributes(QWheelEvent* event);
		/// Return a list of parameters for a context menu event
		static QXmlStreamAttributes contextMenuEventAttributes(QContextMenuEvent* event);
		/// Return a list of parameters for a focus event
		static QXmlStreamAttributes focusEventAttributes(QFocusEvent* event);
};

} // namespace
