#include "Player.h"
#include "../common/ObjectHookName.h"


#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QWidget>

namespace Hooq
{

void Player::sleep(int msec)
{
	QTimer::singleShot(msec, this, SLOT(readNext()));
}

Player::Player(QIODevice* device)
{
	connect(
		device,
		SIGNAL(readyRead()),
		SLOT(readNext())
	);

	setDevice(device);
	readNext();
}

void Player::readNext()
{
	while(!atEnd())
	{
		QXmlStreamReader::readNext();
		if(tokenType() == StartElement)
		{
			handleElement();
			return;
		}
		if(tokenType() == EndDocument)
		{
			emit finished();
			return;
		}
	}
}

void Player::handleElement()
{
	if(name() == "msec")
	{
		sleep(readElementText().toInt());
		return;
	}
	if(name() == "keyPress")
	{
		postKeyEvent(QEvent::KeyPress);
	}
	if(name() == "keyRelease")
	{
		postKeyEvent(QEvent::KeyRelease);
	}
	if(name() == "mouseMove")
	{
		postMouseEvent(QEvent::MouseMove);
	}
	if(name() == "mouseButtonPress")
	{
		postMouseEvent(QEvent::MouseButtonPress);
	}
	if(name() == "mouseButtonDoubleClick")
	{
		postMouseEvent(QEvent::MouseButtonDblClick);
	}
	if(name() == "mouseButtonRelease")
	{
		postMouseEvent(QEvent::MouseButtonRelease);
	}
	readNext();
}

void Player::postKeyEvent(int type)
{
	QObject* object = findObject(attributes().value("target").toString());
	if(!object)
	{
		return;
	}

	QKeyEvent* event = new QKeyEvent(
		static_cast<QEvent::Type>(type),
		attributes().value("key").toString().toInt(),
		static_cast<Qt::KeyboardModifiers>(attributes().value("modifiers").toString().toInt()),
		attributes().value("text").toString(),
		attributes().value("isAutoRepeat") == "true",
		attributes().value("count").toString().toUShort()
	);

	QCoreApplication::postEvent(object, event);
}

void Player::postMouseEvent(int type)
{
	QObject* object = findObject(attributes().value("target").toString());
	if(!object)
	{
		return;
	}

	QMouseEvent* event = new QMouseEvent(
		static_cast<QEvent::Type>(type),
		QPoint(
			attributes().value("x").toString().toInt(),
			attributes().value("y").toString().toInt()
		),
		static_cast<Qt::MouseButton>(attributes().value("button").toString().toInt()),
		static_cast<Qt::MouseButtons>(attributes().value("buttons").toString().toInt()),
		0
	);
	QCoreApplication::postEvent(object, event);
}

QObject* Player::findObject(const QString& path)
{
	const QString separator("::");
	QStringList parts = path.split(separator);
	if(parts.isEmpty())
	{
		return 0;
	}
	const QString name = parts.takeLast();
	QObject* parent = 0;
	if(parts.isEmpty())
	{
		// Top level widget
		Q_FOREACH(QWidget* widget, QApplication::topLevelWidgets())
		{
			if(ObjectHookName::objectName(widget) == name)
			{
				return widget;
			}
		}
		return 0;
	}
	else
	{
		parent = findObject(parts.join(separator));
		if(!parent)
		{
			return 0;
		}
	}
	Q_FOREACH(QObject* child, parent->children())
	{
		if(ObjectHookName::objectName(child) == name)
		{
			return child;
		}
	}
	
	return 0;
}

} // namespace
