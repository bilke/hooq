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
#include "XmlToQtScript.h"

#include "EnumConverter.h"

#include <QDebug>
#include <QStringList>

QString XmlToQtScript::parse(QIODevice* xml, Options options)
{
	m_options = options;

	Q_ASSERT(xml->isOpen() && xml->isReadable());
	setDevice(xml);
	return parseDocument();
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
	QStringList items;
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
				qDebug() << Q_FUNC_INFO << "skipping unknown element" << name();
				skipElement();
				break;
			case EndElement:
				return items.join("\n");
			default:
				break;
		}
	}
	return QString();
}

QString XmlToQtScript::parseMsec()
{
	return QString("msleep(%1);").arg(readElementText());
}

QString XmlToQtScript::escapeString(const QString& _string)
{
	QString string(_string);
	string.replace("\"", "\\\"");
	string.replace("\r", "\\r");
	string.replace("\n", "\\n");
	return string;
}

QString XmlToQtScript::parseFocusEvent()
{
	const QString call("setFocus");
	Q_ASSERT(!call.isEmpty());

	const QString target = attributes().value("target").toString();
	const QString reason = stringForFocusReason(attributes().value("reason").toString().toInt());

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(\"%1\").%2({\"reason\": %3});"
	).arg(
		target
	).arg(
		call
	).arg(
		reason
	);
}

QString XmlToQtScript::parseKeyEvent()
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
	const QString text = escapeString(attributes().value("text").toString());
	const QString autoRepeat = attributes().value("isAutoRepeat").toString();
	const QString count = attributes().value("count").toString();;
	const QString key = "Qt." + EnumConverter::stringFromValue(attributes().value("key").toString().toInt(), "Key");
	const QString modifiers = stringForModifiers(attributes().value("modifiers").toString().toInt());

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(\"%1\").%2({\"key\": %3, \"modifiers\": %4, \"text\": \"%5\", \"autorepeat\": %6, \"count\": %7});"
	).arg(
		target
	).arg(
		call
	).arg(
		key
	).arg(
		modifiers
	).arg(
		text
	).arg(
		autoRepeat
	).arg(
		count
	);
}

QString XmlToQtScript::parseMouseEvent()
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
	const QString x = attributes().value("x").toString();
	const QString y = attributes().value("y").toString();
	const QString button = stringForMouseButton(attributes().value("button").toString().toInt());
	const QString buttons = stringForMouseButtons(attributes().value("buttons").toString().toInt());
	const QString modifiers = stringForModifiers(attributes().value("modifiers").toString().toInt());

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(\"%1\").%2({\"x\": %3, \"y\": %4, \"button\": %5, \"buttons\": %6, \"modifiers\": %7});"
	).arg(
		target
	).arg(
		call
	).arg(
		x
	).arg(
		y
	).arg(
		button
	).arg(
		buttons
	).arg(
		modifiers
	);
}

QString XmlToQtScript::parseContextMenuEvent()
{
	const QString call("contextMenu");

	const QString target = attributes().value("target").toString();
	const QString x = attributes().value("x").toString();
	const QString y = attributes().value("y").toString();
	const QString globalX = attributes().value("globalX").toString();
	const QString globalY = attributes().value("globalY").toString();
	const QString modifiers = stringForModifiers(attributes().value("modifiers").toString().toInt());

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(\"%1\").%2({\"x\": %3, \"y\": %4, \"globalX\": %5, \"globalY\": %6, \"modifiers\": %7});"
	).arg(
		target
	).arg(
		call
	).arg(
		x
	).arg(
		y
	).arg(
		globalX
	).arg(
		globalY
	).arg(
		modifiers
	);
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

QString XmlToQtScript::parseWheelEvent()
{
	const QString call = "mouseWheel";

	const QString target = attributes().value("target").toString();
	const QString x = attributes().value("x").toString();
	const QString y = attributes().value("y").toString();
	const QString orientation = attributes().value("orientation").toString() == "horizontal" ? "Qt.Horizontal" : "Qt.Vertical";
	const QString buttons = stringForMouseButtons(attributes().value("buttons").toString().toInt());
	const QString modifiers = stringForModifiers(attributes().value("modifiers").toString().toInt());
	const QString delta = attributes().value("delta").toString();

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(\"%1\").%2({\"x\": %3, \"y\": %4, \"delta\": %5, \"buttons\": %6, \"modifiers\": %7, \"orientation\": \"%8\"});"
	).arg(
		target
	).arg(
		call
	).arg(
		x
	).arg(
		y
	).arg(
		delta
	).arg(
		buttons
	).arg(
		modifiers
	).arg(
		orientation
	);
}

QString XmlToQtScript::parseShortcutEvent()
{
	const QString call = "shortcut";

	const QString target = attributes().value("target").toString();
	const QString string = attributes().value("string").toString();
	const QString id = attributes().value("id").toString();
	const QString ambiguous = attributes().value("ambiguous").toString();

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(\"%1\").%2({\"string\": \"%3\", \"id\": %4, \"ambiguous\": %5});"
	).arg(
		target
	).arg(
		call
	).arg(
		string
	).arg(
		id
	).arg(
		ambiguous
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
