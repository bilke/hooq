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
#include "ScriptInterface.h"

#include "RemoteObjectPrototype.h"
#include "XmlToQtScript.h"

#include <QDebug>

ScriptInterface* ScriptInterface::m_instance(0);

ScriptInterface::ScriptInterface()
: QObject()
{
}

QScriptValue ScriptInterface::scriptAssert(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(engine);
	const bool success = context->argument(0).toBoolean();
	if(!success)
	{
		return context->throwError(tr("Assertion failed."));
	}
	return QScriptValue();
}

QScriptValue ScriptInterface::scriptSpawnDefaultsAndAttach(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(context);
	Q_UNUSED(engine);
	return QScriptValue();
}

QScriptValue ScriptInterface::scriptRequireHooqScriptVersion(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(engine);
	const int requiredVersion = context->argument(0).toInteger();
	const int supportedVersion = XmlToQtScript::scriptVersion();
	if(requiredVersion > supportedVersion)
	{
		return context->throwError(tr("Script requires HooqScript version %1; this version of Hooq only supports version %2 or older.").arg(requiredVersion).arg(supportedVersion));
	}
	if(requiredVersion < 1)
	{
		return context->throwError(tr("Script requires an invalid version of HooqScript (%1). Version %2 is supported.").arg(requiredVersion).arg(supportedVersion));
	}
	return QScriptValue();
}

QScriptValue ScriptInterface::scriptCompare(QScriptContext* context, QScriptEngine* engine)
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

QScriptValue ScriptInterface::scriptImportExtension(QScriptContext* context, QScriptEngine* engine)
{
	const QString extension = context->argument(0).toString();
	const QScriptValue result = engine->importExtension(extension);
	if(result.isError())
	{
		return context->throwError(tr("Failed to load QtScript extension '%1': %2").arg(extension).arg(result.toString()));
	}
	else
	{
		return QScriptValue();
	}
}

ScriptInterface* ScriptInterface::instance()
{
	if(!m_instance)
	{
		m_instance = new ScriptInterface();
	}
	return m_instance;
}

QScriptValue ScriptInterface::scriptMsleep(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(engine);
	const int msec = context->argument(0).toInteger();
	emit instance()->msleep(msec);
	return QScriptValue();
}

QScriptValue ScriptInterface::scriptObjectFromPath(QScriptContext* context, QScriptEngine* engine)
{
	const QString path = context->argument(0).toString();
	RemoteObjectPrototype* object = new RemoteObjectPrototype(path);
	emit instance()->newRemoteObject(object);
	return engine->newQObject(object, QScriptEngine::ScriptOwnership);
}
