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

#include <QScriptContext>
#include <QScriptEngine>
#include <QScriptValue>

class RemoteApplicationPrototype;
class RemoteObjectPrototype;

class ScriptInterface : public QObject
{
	Q_OBJECT;
	public:
		static ScriptInterface* instance();

		static QScriptValue scriptMsleep(QScriptContext*, QScriptEngine*);
		static QScriptValue scriptObjectFromPath(QScriptContext*, QScriptEngine*);
		static QScriptValue scriptAssert(QScriptContext*, QScriptEngine*);
		static QScriptValue scriptCompare(QScriptContext*, QScriptEngine*);
		static QScriptValue scriptImportExtension(QScriptContext*, QScriptEngine*);
		static QScriptValue scriptRequireHooqScriptVersion(QScriptContext*, QScriptEngine*);
		static QScriptValue scriptSpawnDefaultsAndAttach(QScriptContext*, QScriptEngine*);
	signals:
		void msleep(int msec);
		void newRemoteObject(RemoteObjectPrototype*);
		void newRemoteApplication(RemoteApplicationPrototype*);
	private:
		ScriptInterface();

		static ScriptInterface* m_instance;
};
