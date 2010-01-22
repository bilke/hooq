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

class ObjectInformation;
class RemoteApplicationPrototype;
class RemoteObjectPrototype;

#include <QObject>
#include <QXmlStreamWriter>

class QKeySequence;
class QPoint;
class QTcpSocket;
class QScriptContext;
class QScriptEngine;
 
class Interpreter : public QObject, private QXmlStreamWriter
{
	Q_OBJECT;
	public:
		Interpreter(QObject* parent);
		~Interpreter();
		QScriptEngine* engine() const;
		bool haveRequiredQtScriptExtensions() const;
		void setScriptPath(const QString& scriptPath);
		static int lineNumber(QScriptContext* context);
	signals:
		void objectPicked(const ObjectInformation&);
		void finished();
		void executionFailed(int lineNumber);
		void objectNotFound(const QString& path);
		void startApplicationAndAttach();
		void startApplicationAndAttach(const QString& path, const QStringList& arguments);
		void closeApplication();
	public slots:
		void run();
		void setSocket(QTcpSocket* socket);
		void processSocketData();
		void pickObject();
	private slots:
		void connectRemoteApplication(RemoteApplicationPrototype*);
		void connectRemoteObject(RemoteObjectPrototype*);
		void writeCloseEvent(const QString& path);
		void writeContextMenuEvent(const QString& path, const QPoint& position, const QPoint& globalPosition, Qt::KeyboardModifiers modifiers);
		void writeSetFocusEvent(const QString& path, Qt::FocusReason reason);
		void writeKeyPressEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void writeKeyReleaseEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void writeMouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeMousePressEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeMouseReleaseEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeMouseDoubleClickEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void writeShortcutEvent(const QString& path, const QKeySequence& sequence, int id, bool ambiguous);
		void writeSleep(int msec);
		void writeWheelEvent(const QString& path, const QPoint& position, int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::Orientation orientation);
		void writeDragAndDropEvent(const QString& sourcePath, const QPoint& sourcePosition, const QString& targetPath, const QPoint& targetPosition);
		void fetchProperty(const QString& path, const QString& name, QVariant* value);
	private:
		bool importExtension(const QString& extension);

		bool ack() const;
		bool waitForAck();
		void waitForDumpedObject();

		void setAck(bool ack = true);
		void writeKeyAttributes(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void writeMouseAttributes(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);

		ObjectInformation* m_dumpedObject;
		int m_pendingAcks;

		enum AttachState
		{
			NotAttached,
			WaitingForAttach,
			Attached
		};
		AttachState m_attachState;
		void waitForAttach();
		void ensureAttached();

		bool m_haveRequiredQtScriptExtensions;
		QScriptEngine* m_engine;
		QString m_script;
		QString m_scriptPath;
};
