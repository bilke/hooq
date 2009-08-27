#include "XmlToQtScript.h"

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
	const QString text = attributes().value("text").toString();
	const QString autoRepeat = attributes().value("isAutoRepeat").toString();
	const QString count = attributes().value("count").toString();;
	// TODO: enums
	const QString key = attributes().value("key").toString();
	const QString modifiers = attributes().value("modifiers").toString();

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(\"%1\")->%2(%3, %4, \"%5\", %6, %7);"
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
	// TODO: enums
	const QString button = attributes().value("button").toString();
	const QString buttons = attributes().value("buttons").toString();;
	const QString modifiers = attributes().value("modifiers").toString();

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(%1)->%2(QPoint(%3, %4), %5, %6, %7);"
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

QString XmlToQtScript::parseWheelEvent()
{
	const QString call = "mouseWheel";

	const QString target = attributes().value("target").toString();
	const QString x = attributes().value("x").toString();
	const QString y = attributes().value("y").toString();
	// TODO: enums
	const QString delta = attributes().value("delta").toString();
	const QString buttons = attributes().value("buttons").toString();;
	const QString modifiers = attributes().value("modifiers").toString();
	const QString orientation = attributes().value("orientation").toString();

	// skip to end of element
	readElementText();

	return QString(
		"objectFromPath(%1)->%2(QPoint(%3, %4), %5, %6, %7, %8);"
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
