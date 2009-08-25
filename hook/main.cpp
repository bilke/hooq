#include <Qt>

#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QString>
#include <QStringList>

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

	return QString(
		"%1:%2"
	).arg(
		object->metaObject()->className()
	).arg(
		object->parent()->children().indexOf(object)
	);
}

QString objectPath(QObject* object)
{
	QStringList components;
	QObject* current = object;
	while(current)
	{
		components.prepend(objectName(current));
		current = current->parent();
	}
	return components.join("/");
}

bool myHook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	qDebug() << objectPath(receiver) << event->type();
	return false;
}

extern "C" void registerFredhookCallback()
{
	QInternal::registerCallback(QInternal::EventNotifyCallback, &myHook);
}
