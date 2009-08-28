#include "XmlToQtScript.h"

#include "EnumConverter.h"

#include <QDebug>
#include <QStringList>

QString XmlToQtScript::parse(QIODevice* xml)
{
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
				if(name() == "keyPress" || name() == "keyRelease")
				{
					items.append(parseKeyEvent());
					continue;
				}
				if(name() == "mouseMove" || name() == "mouseButtonPress" || name() == "mouseButtonRelease")
				{
					items.append(parseMouseEvent());
					continue;
				}
				if(name() == "mouseWheel")
				{
					items.append(parseWheelEvent());
					continue;
				}
				qDebug() << Q_FUNC_INFO << "skipping unknown element" << name();
				skipElement();
				break;
			case EndElement:
				return items.join("\n") + "\n";
			default:
				break;
		}
	}
	return QString();
}

QString XmlToQtScript::parseMsec()
{
	return QString("usleep(%1);").arg(readElementText());
}

QString XmlToQtScript::escapeString(const QString& _string)
{
	QString string(_string);
	string.replace("\"", "\\\"");
	string.replace("\r", "\\r");
	return string;
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
