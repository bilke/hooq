#include "GlobalEventHook.h"

#include <Qt>

#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QStringList>
#include <QTextStream>
#include <QTime>
#include <QUrl>

QFile GlobalEventHook::m_logFile;

void GlobalEventHook::setLogFile(const QString& targetFile)
{
	m_logFile.close();
	m_logFile.setFileName(targetFile);
	m_logFile.open(QIODevice::WriteOnly | QFile::Truncate);
}

void GlobalEventHook::activate()
{
	// Internal API... seems unavoidable
	QInternal::registerCallback(QInternal::EventNotifyCallback, &hook);
}

bool GlobalEventHook::hook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	// returnType* returnValue = reinterpret_cast<returnType*>(data[2])
	switch(event->type())
	{
		case QEvent::KeyPress:
			outputEvent(objectPath(receiver), "keyPress", formattedKeyEvent(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::KeyRelease:
			outputEvent(objectPath(receiver), "keyRelease", formattedKeyEvent(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::MouseMove:
			outputEvent(objectPath(receiver), "mouseMove", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonPress:
			outputEvent(objectPath(receiver), "mouseButtonPress", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonRelease:
			outputEvent(objectPath(receiver), "mouseButtonRelease", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonDblClick:
			outputEvent(objectPath(receiver), "mouseButtonDoubleClick", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		default:
			break;

	}
	return false;
}

QList<QPair<QString, QString> > GlobalEventHook::formattedKeyEvent(QKeyEvent* event)
{
	QList<QPair<QString, QString> > data;
	data.append(qMakePair(QString("key"), QString::number(event->key()))); // Qt::Key
	data.append(qMakePair(QString("modifiers"), QString::number(event->modifiers()))); // Qt::Modifiers
	return data;
}

QList<QPair<QString, QString> > GlobalEventHook::formattedMouseEvent(QMouseEvent* event)
{
	QList<QPair<QString, QString> > data;
	data.append(qMakePair(QString("x"), QString::number(event->x())));
	data.append(qMakePair(QString("y"), QString::number(event->y())));
	data.append(qMakePair(QString("button"), QString::number(event->button())));
	data.append(qMakePair(QString("buttons"), QString::number(event->buttons())));
	return data;
}

QString GlobalEventHook::objectName(QObject* object)
{
	// Grab the object name
	if(!object->objectName().isEmpty())
	{
		return object->objectName();
	}

	// If it's got no parent, classname:0
	if(!object->parent())
	{
		return QString("%1:0").arg(object->metaObject()->className());
	}

	// It does - classname:Index
	const QList<QObject*> siblings = object->parent()->children();

	int index = 1;
	Q_FOREACH(QObject* sibling, siblings)
	{
		if(sibling == object)
		{
			break;
		}
		if(sibling->metaObject() == object->metaObject())
		{
			++index;
		}
	}

	return QString(
		"%1:%2"
	).arg(
		object->metaObject()->className()
	).arg(
		index
	);
}

QString GlobalEventHook::objectPath(QObject* object)
{
	QStringList components;
	QObject* current = object;
	while(current)
	{
		components.append(objectName(current));
		current = current->parent();
	}
	return components.join(".");
}

void GlobalEventHook::outputEvent(const QString& object, const char* action, const QList<QPair<QString, QString> >& data)
{
	QTextStream out(&m_logFile);
	static QTime timer;
	if(timer.isValid())
	{
		QUrl timeUrl;
		timeUrl.setScheme("msec");
		timeUrl.setHost(QString::number(timer.restart()));
		out << qPrintable(timeUrl.toString()) << endl;
	}
	else
	{
		timer.start();
	}
	QUrl url;
	url.setScheme("qevent");
	url.setHost(object);
	url.setPath(action);
	url.setQueryItems(data);
	out << qPrintable(url.toString()) << endl;
}
