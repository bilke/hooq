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
#include "XmlToQtScript.h"
#include "XmlToQtScript_Item_Inserter.h"
#include "XmlToQtScript_FilterQtInternalsPostProcessor.h"
#include "XmlToQtScript_MouseMovePostProcessor.h"
#include "XmlToQtScript_ObjectVariablesPostProcessor.h"
#include "XmlToQtScript_SimplifyMouseMovementsPostProcessor.h"
#include "XmlToQtScript_SimplifyStringsPostProcessor.h"
#include "XmlToQtScript_StringVariablesPostProcessor.h"

#include "EnumConverter.h"

#include <QDebug>
#include <QStringList>

Q_DECLARE_METATYPE(Qt::FocusReason);
Q_DECLARE_METATYPE(Qt::Key);
Q_DECLARE_METATYPE(Qt::MouseButton);
Q_DECLARE_METATYPE(Qt::MouseButtons);
Q_DECLARE_METATYPE(Qt::Orientation);
Q_DECLARE_METATYPE(Qt::KeyboardModifier);
Q_DECLARE_METATYPE(Qt::KeyboardModifiers);

XmlToQtScript::Variable::Variable()
{
}

XmlToQtScript::Variable::Variable(const QString& _name)
: name(_name)
{
}

int XmlToQtScript::scriptVersion()
{
	// bump whenever compatibility changes
	return 2;
}

QString XmlToQtScript::parse(QIODevice* xml, Options options)
{
	m_options = options;

	Q_ASSERT(xml->isOpen() && xml->isReadable());
	setDevice(xml);
	return QString("requireHooqScriptVersion(%1);\n").arg(scriptVersion()) + parseDocument();
}

QString XmlToQtScript::parseDocument()
{
	while(!atEnd())
	{
		readNext();
		if(tokenType() == StartElement)
		{
			if(name() == "hooq")
			{
				return parseHooq();
			}
			skipElement();
		}
	}
	return QString();
}

void XmlToQtScript::skipElement()
{
	Q_ASSERT(tokenType() == StartElement);
	while(!atEnd())
	{
		readNext();
		switch(tokenType())
		{
			case StartElement:
				skipElement();
				break;
			case EndElement:
				return;
			default:
				break;
		}
	}
}

QString XmlToQtScript::parseHooq()
{
	QList<Item> items;
	while(!atEnd())
	{
		readNext();
		switch(tokenType())
		{
			case StartElement:
				if(name() == "msec")
				{
					items.append(parseMsec());
					continue;
				}
				if(name() == "windowClosed")
				{
					items.append(parseCloseEvent());
					continue;
				}
				if(name() == "focusChanged")
				{
					items.append(parseFocusEvent());
					continue;
				}
				if(name() == "keyPress" || name() == "keyRelease")
				{
					items.append(parseKeyEvent());
					continue;
				}
				if(name() == "mouseMove" || name() == "mouseButtonPress" || name() == "mouseButtonRelease" || name() == "mouseButtonDoubleClick")
				{
					items.append(parseMouseEvent());
					continue;
				}
				if(name() == "mouseWheel")
				{
					items.append(parseWheelEvent());
					continue;
				}
				if(name() == "shortcut")
				{
					items.append(parseShortcutEvent());
					continue;
				}
				if(name() == "contextMenu")
				{
					items.append(parseContextMenuEvent());
					continue;
				}
				if(name() == "dragAndDrop")
				{
					items.append(parseDragAndDropEvent());
					continue;
				}
				skipElement();
				break;
			case EndElement:
				return itemString(items);
			default:
				break;
		}
	}
	qDebug() << "Unexpected EOF in XML";
	return QString("/* Unexpected EOF in XML */\n" + itemString(items));
}

QString XmlToQtScript::itemString(const QList<Item>& items) const
{
	QList<Item> in(items);
	QList<Item> out;

	XmlToQtScript::Item::Inserter inserter(&out);

	QList<XmlToQtScript::PostProcessor*> postProcessors;
	if(m_options & FilterQtInternals)
	{
		postProcessors.append(new FilterQtInternalsPostProcessor());
	}
	if(m_options & SkipMouseMovements)
	{
		postProcessors.append(new StripMouseMovementsPostProcessor());
	}
	if(m_options & SimplifyMouseMovements)
	{
		postProcessors.append(new SimplifyMouseMovementsPostProcessor(&inserter));
	}
	if(m_options & SimplifyStrings)
	{
		postProcessors.append(new SimplifyStringsPostProcessor(&inserter));
	}
	if(m_options & ObjectVariables)
	{
		postProcessors.append(new ObjectVariablesPostProcessor(&inserter));
	}
	if(m_options & StringVariables)
	{
		postProcessors.append(new StringVariablesPostProcessor(&inserter));
	}

	Q_FOREACH(PostProcessor* postProcessor, postProcessors)
	{
		out.clear();
		while(!in.isEmpty())
		{
			Item item = in.takeFirst();
			postProcessor->process(&item, &in);
			if(item.isValid())
			{
				out.append(item);
			}
			inserter.next();
		}
		in = out;
		inserter.reset();
	}

	qDeleteAll(postProcessors);

	// Serialise
	return serialize(in);
}

QString XmlToQtScript::serialize(const QList<Item>& items, bool isStatement) const
{
	QStringList out;
	QString suffix;
	if(isStatement)
	{
		suffix = ";";
	}
	Q_FOREACH(const Item& item, items)
	{
		if(item.method.isNull())
		{
			continue;
		}

		if(item.target.isNull())
		{
			if(item.parameters.isNull())
			{
				out.append(item.method + suffix);
			}
			else
			{
				out.append(QString("%1(%2)").arg(item.method).arg(parametersString(item.parameters)) + suffix);
			}
		}
		else
		{
			if(item.target.canConvert<Variable>())
			{
				out.append(QString("%1.%2(%3)").arg(item.target.value<Variable>().name).arg(item.method).arg(parametersString(item.parameters)) + suffix);
			}
			else
			{
				out.append(QString("objectFromPath(\"%1\").%2(%3)").arg(item.target.toString()).arg(item.method).arg(parametersString(item.parameters)) + suffix);
			}
		}
	}
	return out.join("\n");
}

QString XmlToQtScript::parametersString(const QVariant& parameters) const
{
	switch(parameters.type())
	{
		case QVariant::Bool:
			return parameters.toBool() ? "true" : "false";
		case QVariant::Int:
			return QString::number(parameters.toInt());
		case QVariant::String:
			return "\"" + escapeString(parameters.toString()) + "\"";
		case QVariant::Map:
			{
				QVariantMap map = parameters.toMap();
				QStringList out;
				for(QVariantMap::ConstIterator it = map.constBegin(); it != map.constEnd(); ++it)
				{
					out += QString("\"%1\": %2").arg(escapeString(it.key())).arg(parametersString(it.value()));
				}
				return "{" + out.join(", ") + "}";
			}
		case QVariant::Invalid:
			return QString();
		case QVariant::UserType:
			if(parameters.canConvert<Qt::FocusReason>())
			{
				return stringForFocusReason(parameters.value<Qt::FocusReason>());
			}
			if(parameters.canConvert<Qt::Key>())
			{
				return "Qt." + EnumConverter::stringFromValue(parameters.value<Qt::Key>(), "Key");
			}
			if(parameters.canConvert<Qt::MouseButton>())
			{
				return stringForMouseButton(parameters.value<Qt::MouseButton>());
			}
			if(parameters.canConvert<Qt::MouseButtons>())
			{
				return stringForMouseButtons(parameters.value<Qt::MouseButtons>());
			}
			if(parameters.canConvert<Qt::Orientation>())
			{
				return parameters.value<Qt::Orientation>() == Qt::Horizontal ? "Qt.Horizontal" : "Qt.Vertical";
			}
			if(parameters.canConvert<Qt::KeyboardModifier>())
			{
				return stringForModifier(parameters.value<Qt::KeyboardModifier>());
			}
			if(parameters.canConvert<Qt::KeyboardModifiers>())
			{
				return stringForModifiers(parameters.value<Qt::KeyboardModifiers>());
			}
			if(parameters.canConvert<Variable>())
			{
				return parameters.value<Variable>().name;
			}
			if(parameters.canConvert<Item>())
			{
				return serialize(QList<Item>() << parameters.value<Item>(), false);
			}
			qDebug() << "Unhandled usertype:" << parameters.userType() << parameters.typeName();
			break;
		default:
			qDebug() << "Unhandled standard QVariant type:" << parameters.typeName();
	}
	return QString();
}

XmlToQtScript::Item::Item(const QVariant& _target, const QString& _targetClass, const QString& _method, const QVariant& _parameters)
: target(_target)
, targetClass(_targetClass)
, method(_method)
, parameters(_parameters)
{
}

XmlToQtScript::Item::Item(const QString& _method, const QVariant& _parameters)
: method(_method)
, parameters(_parameters)
{
}

XmlToQtScript::Item::Item()
{
}

bool XmlToQtScript::Item::isValid() const
{
	return !method.isEmpty();
}

XmlToQtScript::Item XmlToQtScript::parseMsec()
{
	return Item("msleep", readElementText().toInt());
}

QString XmlToQtScript::escapeString(const QString& _string)
{
	QString string(_string);
	string.replace("\\", "\\\\");
	string.replace("\"", "\\\"");
	string.replace("\r", "\\r");
	string.replace("\n", "\\n");
	return string;
}

XmlToQtScript::Item XmlToQtScript::parseFocusEvent()
{
	const QString call("setFocus");
	Q_ASSERT(!call.isEmpty());

	const QString target = attributes().value("target").toString();
	const QString targetClass = attributes().value("targetClass").toString();

	QVariantMap parameters;
	parameters["reason"] = QVariant::fromValue(static_cast<Qt::FocusReason>(attributes().value("reason").toString().toInt()));

	// skip to end of element
	readElementText();

	return Item(target, targetClass, call, parameters);
}

XmlToQtScript::Item XmlToQtScript::parseCloseEvent()
{
	const QString call("close");
	Q_ASSERT(!call.isEmpty());

	const QString target = attributes().value("target").toString();
	const QString targetClass = attributes().value("targetClass").toString();

	const QVariant parameters; // none at the moment

	// skip to end of element
	readElementText();

	return Item(target, targetClass, call, parameters);
}

XmlToQtScript::Item XmlToQtScript::parseKeyEvent()
{
	QString call;
	if(name() == "keyPress")
	{
		call = "pressKey";
	}
	else if(name() == "keyRelease")
	{
		call = "releaseKey";
	}
	Q_ASSERT(!call.isEmpty());

	const QString target = attributes().value("target").toString();
	const QString targetClass = attributes().value("targetClass").toString();

	QVariantMap parameters;

	parameters["key"] = QVariant::fromValue(static_cast<Qt::Key>(attributes().value("key").toString().toInt()));
	parameters["text"] = attributes().value("text").toString();
	parameters["autoRepeat"] = attributes().value("isAutoRepeat").toString() == "true" ? true : false;
	parameters["count"] = attributes().value("count").toString().toInt();
	parameters["modifiers"] = QVariant::fromValue(Qt::KeyboardModifiers(attributes().value("modifiers").toString().toInt()));

	// skip to end of element
	readElementText();

	return Item(
		target,
		targetClass,
		call,
		parameters
	);
}

XmlToQtScript::Item XmlToQtScript::parseMouseEvent()
{
	QString call;
	if(name() == "mouseMove")
	{
		call = "moveMouse";
	}
	else if(name() == "mouseButtonDoubleClick")
	{
		call = "doubleClickMouseButton";
	}
	else if(name() == "mouseButtonPress")
	{
		call = "pressMouseButton";
	}
	else if(name() == "mouseButtonRelease")
	{
		call = "releaseMouseButton";
	}
	Q_ASSERT(!call.isEmpty());

	const QString target = attributes().value("target").toString();
	const QString targetClass = attributes().value("targetClass").toString();

	QVariantMap parameters;

	parameters["x"] = attributes().value("x").toString().toInt();
	parameters["y"] = attributes().value("y").toString().toInt();
	parameters["button"] = QVariant::fromValue(static_cast<Qt::MouseButton>(attributes().value("button").toString().toInt()));
	parameters["buttons"] = QVariant::fromValue(Qt::MouseButtons(attributes().value("buttons").toString().toInt()));
	parameters["modifiers"] = QVariant::fromValue(Qt::KeyboardModifiers(attributes().value("modifiers").toString().toInt()));

	// skip to end of element
	readElementText();

	return Item(
		target,
		targetClass,
		call,
		parameters
	);
}

XmlToQtScript::Item XmlToQtScript::parseContextMenuEvent()
{
	const QString call("contextMenu");

	const QString target = attributes().value("target").toString();
	const QString targetClass = attributes().value("targetClass").toString();

	QVariantMap parameters;
	parameters["x"] = attributes().value("x").toString().toInt();
	parameters["y"] = attributes().value("y").toString().toInt();
	parameters["globalX"] = attributes().value("globalX").toString().toInt();
	parameters["globalY"] = attributes().value("globalY").toString().toInt();
	parameters["modifiers"] = QVariant::fromValue(Qt::KeyboardModifiers(attributes().value("modifiers").toString().toInt()));

	// skip to end of element
	readElementText();

	return Item(
		target,
		targetClass,
		call,
		parameters
	);
}

XmlToQtScript::Item XmlToQtScript::parseDragAndDropEvent()
{
	// Two items, one nested as a parameter of the other:
	// - The Drag item
	// - The Drop item
	
	// First, construct the drag item
	Item dragItem;
	{
		const QString call("drag");
		const QString target(attributes().value("source").toString());
		QVariantMap parameters;
		parameters["x"] = attributes().value("sourceX").toString().toInt();
		parameters["y"] = attributes().value("sourceY").toString().toInt();
		dragItem = Item(target, QString(), call, parameters);
	}
	// Now, the drop item
	Item dropItem;
	{
		const QString call("drop");
		const QString target(attributes().value("target").toString());
		QVariantMap parameters;
		parameters["x"] = attributes().value("targetX").toString().toInt();
		parameters["y"] = attributes().value("targetY").toString().toInt();
		parameters["dragged"] = QVariant::fromValue(dragItem);
		dropItem = Item(target, QString(), call, parameters);
	}
	// Skip to end of the element
	readElementText();
	return dropItem;
}

QString XmlToQtScript::stringForMouseButton(int button)
{
	switch(button)
	{
		case Qt::LeftButton:
			return "Qt.LeftButton";
		case Qt::RightButton:
			return "Qt.RightButton";
		case Qt::MidButton:
			return "Qt.MidButton";
		case Qt::XButton1:
			return "Qt.XButton1";
		case Qt::XButton2:
			return "Qt.XButton2";
	}
	return "Qt.NoButton";
}

QString XmlToQtScript::stringForModifier(int modifier)
{
	switch(modifier)
	{
		case Qt::ShiftModifier:
			return "Qt.ShiftModifier";
		case Qt::ControlModifier:
			return "Qt.ControlModifier";
		case Qt::AltModifier:
			return "Qt.AltModifier";
		case Qt::MetaModifier:
			return "Qt.MetaModifier";
		case Qt::KeypadModifier:
			return "Qt.KeypadModifier";
		case Qt::GroupSwitchModifier:
			return "Qt.GroupSwitchModifier";
	}
	return "Qt.NoModifier";
}

QString XmlToQtScript::stringForFocusReason(int focusReason)
{
	switch(focusReason)
	{
		case Qt::MouseFocusReason:
			return "Qt.MouseFocusReason";
		case Qt::TabFocusReason:
			return "Qt.TabFocusReason";
		case Qt::BacktabFocusReason:
			return "Qt.BacktabFocusReason";
		case Qt::ActiveWindowFocusReason:
			return "Qt.ActiveWindowFocusReason";
		case Qt::PopupFocusReason:
			return "Qt.PopupFocusReason";
		case Qt::ShortcutFocusReason:
			return "Qt.ShortcutFocusReason";
		case Qt::MenuBarFocusReason:
			return "Qt.MenuBarFocusReason";
		case Qt::OtherFocusReason:
			return "Qt.OtherFocusReason";
	}
	return "Qt.OtherFocusReason";
}

XmlToQtScript::Item XmlToQtScript::parseWheelEvent()
{
	const QString call = "mouseWheel";

	const QString target = attributes().value("target").toString();
	const QString targetClass = attributes().value("targetClass").toString();

	QVariantMap parameters;

	parameters["x"] = attributes().value("x").toString().toInt();
	parameters["y"] = attributes().value("y").toString().toInt();
	parameters["buttons"] = QVariant::fromValue(Qt::MouseButtons(attributes().value("buttons").toString().toInt()));
	parameters["modifiers"] = QVariant::fromValue(Qt::KeyboardModifiers(attributes().value("modifiers").toString().toInt()));
	parameters["orientation"] = QVariant::fromValue(static_cast<Qt::Orientation>(attributes().value("orientation").toString() == "horizontal" ? Qt::Horizontal : Qt::Vertical));
	parameters["delta"] = attributes().value("delta").toString().toInt();

	// skip to end of element
	readElementText();

	return Item(
		target,
		targetClass,
		call,
		parameters
	);
}

XmlToQtScript::Item XmlToQtScript::parseShortcutEvent()
{
	const QString call = "shortcut";

	const QString target = attributes().value("target").toString();
	const QString targetClass = attributes().value("targetClass").toString();

	QVariantMap parameters;

	parameters["string"] = attributes().value("string").toString();
	parameters["id"] = attributes().value("id").toString().toInt();
	parameters["ambiguous"] = attributes().value("ambiguous").toString() == "true" ? true : false;

	// skip to end of element
	readElementText();

	return Item(
		target,
		targetClass,
		call,
		parameters
	);
}

QString XmlToQtScript::stringForModifiers(int modifiers)
{
	QStringList out;
	unsigned int bits = sizeof(int) * 8;
	for(unsigned int i = 0; i < bits; ++i)
	{
		const int modifier = 1 << i;
		if(modifiers & modifier)
		{
			out.append(stringForModifier(modifier));
		}
	}
	if(out.isEmpty())
	{
		out.append(stringForModifier(Qt::NoModifier));
	}
	return out.join(" | ");
}

QString XmlToQtScript::stringForMouseButtons(int buttons)
{
	QStringList out;
	unsigned int bits = sizeof(int) * 8;
	for(unsigned int i = 0; i < bits; ++i)
	{
		const int button = 1 << i;
		if(buttons & button)
		{
			out.append(stringForMouseButton(button));
		}
	}
	if(out.isEmpty())
	{
		out.append(stringForMouseButton(Qt::NoButton));
	}
	return out.join(" | ");
}
