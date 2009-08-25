#include "ObjectHookName.h"

#include <QObject>
#include <QStringList>

QString ObjectHookName::objectName(QObject* object)
{
	// Grab the object name
	if(!object->objectName().isEmpty())
	{
		return object->objectName().toLower();
	}

	// If it's got no parent, classname:0
	if(!object->parent())
	{
		return QString("%1-0").arg(object->metaObject()->className()).toLower();
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
		"%1-%2"
	).arg(
		object->metaObject()->className()
	).arg(
		index
	).toLower();
}

QString ObjectHookName::objectPath(QObject* object)
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
