#include <Qt>

#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>

#include <iostream>

QString objectName(QObject* object)
{
	if(!object->objectName().isEmpty())
	{
		return object->objectName();
	}

	if(!object->parent())
	{
		return QString("%1:1").arg(object->metaObject()->className());
	}

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

QString objectPath(QObject* object)
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

QList<QPair<QString, QString> > formattedKeyEvent(QKeyEvent* event)
{
	QList<QPair<QString, QString> > data;
	data.append(qMakePair(QString("key"), QString::number(event->key())));
	data.append(qMakePair(QString("modifiers"), QString::number(event->modifiers())));
	return data;
}

QList<QPair<QString, QString> > formattedMouseEvent(QMouseEvent* event)
{
	QList<QPair<QString, QString> > data;
	data.append(qMakePair(QString("x"), QString::number(event->x())));
	data.append(qMakePair(QString("y"), QString::number(event->y())));
	data.append(qMakePair(QString("button"), QString::number(event->button())));
	data.append(qMakePair(QString("buttons"), QString::number(event->buttons())));
	return data;
}

void outputEvent(const QString& object, const char* action, const QList<QPair<QString, QString> >& data)
{
	QUrl url;
	url.setScheme("qevent");
	url.setHost(object);
	url.setPath(action);
	url.setQueryItems(data);
	std::cout << qPrintable(url.toString()) << std::endl;
}

bool myHook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
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

extern "C" void registerFredhookCallback()
{
	QInternal::registerCallback(QInternal::EventNotifyCallback, &myHook);
}
