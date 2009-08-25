#include "Player.h"
#include "../lib/ObjectHookName.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStringList>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <QWidget>

namespace Hooq
{

QFile Player::m_logFile;

void Player::sleep(int msec)
{
	QTimer::singleShot(msec, this, SLOT(readNext()));
}

void Player::run()
{
	Player* player = new Player();
	player->m_logStream.setDevice(&m_logFile);
	player->readNext();
}

void Player::readNext()
{
	const QString line = m_logStream.readLine();
	if(line.isNull())
	{
		return;
	}
	const QUrl url(line);
	if(url.scheme() == "msec")
	{
		sleep(url.host().toInt());
		return;
	}
	if(url.scheme() == "qevent")
	{
		QObject* receiver = findObject(url.host());
		if(!receiver)
		{
			qDebug() << "Couldn't find receiver for" << url;
			readNext();
			return;
		}
		if(url.path() == "/keyPress")
		{
			postKeyEvent(receiver, QEvent::KeyPress, url);
			readNext();
			return;
		}
		if(url.path() == "/keyRelease")
		{
			postKeyEvent(receiver, QEvent::KeyRelease, url);
			readNext();
			return;
		}
		if(url.path() == "/mouseMove")
		{
			postMouseEvent(receiver, QEvent::MouseMove, url);
			readNext();
			return;
		}
		if(url.path() == "/mouseButtonPress")
		{
			postMouseEvent(receiver, QEvent::MouseButtonPress, url);
			readNext();
			return;
		}
		if(url.path() == "/mouseButtonDoubleClick")
		{
			postMouseEvent(receiver, QEvent::MouseButtonDblClick, url);
			readNext();
			return;
		}
		if(url.path() == "/mouseButtonRelease")
		{
			postMouseEvent(receiver, QEvent::MouseButtonRelease, url);
			readNext();
			return;
		}
	}
}

void Player::postKeyEvent(QObject* object, int type, const QUrl& url)
{
	QKeyEvent* event = new QKeyEvent(
		static_cast<QEvent::Type>(type),
		url.queryItemValue("key").toInt(),
		static_cast<Qt::KeyboardModifiers>(url.queryItemValue("modifiers").toInt()),
		url.queryItemValue("text"),
		url.queryItemValue("isAutoRepeat") == "true",
		url.queryItemValue("count").toUShort()
	);
	QCoreApplication::postEvent(object, event);
}


void Player::postMouseEvent(QObject* object, int type, const QUrl& url)
{
	QMouseEvent* event = new QMouseEvent(
		static_cast<QEvent::Type>(type),
		QPoint(
			url.queryItemValue("x").toInt(),
			url.queryItemValue("y").toInt()
		),
		static_cast<Qt::MouseButton>(url.queryItemValue("button").toInt()),
		static_cast<Qt::MouseButtons>(url.queryItemValue("buttons").toInt()),
		0
	);
	QCoreApplication::postEvent(object, event);
}

void Player::setLogFile(const QString& targetFilePath)
{
	m_logFile.close();
	m_logFile.setFileName(targetFilePath);
	m_logFile.open(QIODevice::ReadOnly);
}

QObject* Player::findObject(const QString& path)
{
	QStringList parts = path.split(".");
	if(parts.isEmpty())
	{
		return 0;
	}
	const QString name = parts.takeFirst();
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
		parent = findObject(parts.join("."));
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
