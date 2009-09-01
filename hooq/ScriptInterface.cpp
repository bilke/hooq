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
#include "ScriptInterface.h"

#include "RemoteObjectPrototype.h"

#include <QDebug>

ScriptInterface* ScriptInterface::m_instance(0);

ScriptInterface::ScriptInterface()
: QObject()
{
}

QScriptValue ScriptInterface::assert(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(engine);
	const bool success = context->argument(0).toBool();
	if(!success)
	{
		return context->throwError(tr("Assertion failed."));
	}
	return QScriptValue();
}

QScriptValue ScriptInterface::compare(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(engine);
	const QVariant firstValue = context->argument(0).toVariant();
	const QVariant secondValue = context->argument(1).toVariant();

	if(firstValue != secondValue)
	{
		return context->throwError(tr("Comparison failed. Got: '%1'. Expected: '%2'").arg(firstValue.toString()).arg(secondValue.toString()));
	}
	return QScriptValue();
}

ScriptInterface* ScriptInterface::instance()
{
	if(!m_instance)
	{
		m_instance = new ScriptInterface();
	}
	return m_instance;
}

QScriptValue ScriptInterface::msleep(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(engine);
	const int msec = context->argument(0).toInteger();
	emit instance()->msleep(msec);
	return QScriptValue();
}

QScriptValue ScriptInterface::objectFromPath(QScriptContext* context, QScriptEngine* engine)
{
	const QString path = context->argument(0).toString();
	RemoteObjectPrototype* object = new RemoteObjectPrototype(path);
	emit instance()->newRemoteObject(object);
	return engine->newQObject(object, QScriptEngine::ScriptOwnership);
}
