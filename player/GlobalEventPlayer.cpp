#include "GlobalEventPlayer.h"
#include "../lib/ObjectHookName.h"

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QStringList>
#include <QTextStream>
#include <QTime>
#include <QUrl>
#include <QWidget>

QFile GlobalEventPlayer::m_logFile;
QHash<QString, QPointer<QObject> > GlobalEventPlayer::m_objectCache;

void GlobalEventPlayer::sleep(int msec)
{
	QTime time;
	time.start();
	do
	{
		QCoreApplication::processEvents();
	}
	while(time.elapsed() < msec);
}

void GlobalEventPlayer::run()
{
	QTextStream in(&m_logFile);
	Q_FOREVER
	{
		const QString line = in.readLine();
		if(line.isNull())
		{
			break;
		}
		const QUrl url(line);
		if(url.scheme() == "msec")
		{
			sleep(url.host().toInt());
			continue;
		}
		if(url.scheme() == "qevent")
		{
			qDebug() << "Looking for" << url.host();
			QObject* receiver = findObject(url.host());
			if(!receiver)
			{
				qDebug() << "Couldn't find receiver for" << url;
				continue;
			}
			if(url.path() == "/mouseMove")
			{
				postMouseEvent(receiver, QEvent::MouseMove, url);
				continue;
			}
			if(url.path() == "/mouseButtonPress")
			{
				postMouseEvent(receiver, QEvent::MouseButtonPress, url);
				continue;
			}
			if(url.path() == "/mouseButtonRelease")
			{
				postMouseEvent(receiver, QEvent::MouseButtonRelease, url);
				continue;
			}
		}
	}
}

void GlobalEventPlayer::postMouseEvent(QObject* object, int type, const QUrl& url)
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

void GlobalEventPlayer::setLogFile(const QString& targetFilePath)
{
	m_logFile.close();
	m_logFile.setFileName(targetFilePath);
	m_logFile.open(QIODevice::ReadOnly);
}

QObject* GlobalEventPlayer::findObject(const QString& path)
{
	qDebug() << Q_FUNC_INFO << path;
	QStringList parts = path.split(".");
	if(parts.isEmpty())
	{
		return 0;
	}
	const QString name = parts.takeFirst();
	QObject* parent = 0;
	if(parts.isEmpty())
	{
		qDebug() << "Looking for top-level for" << path;
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
		qDebug() << "Looking for parent for" << path;
		parent = findObject(parts.join("."));
		if(!parent)
		{
			return 0;
		}
	}
	qDebug() << "Looking at children for" << path;
	Q_FOREACH(QObject* child, parent->children())
	{
		qDebug() << ObjectHookName::objectName(child) << name;
		if(ObjectHookName::objectName(child) == name)
		{
			return child;
		}
	}
	
	return 0;
}
