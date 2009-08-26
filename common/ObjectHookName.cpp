#include "ObjectHookName.h"

#include <QObject>
#include <QStringList>

namespace Hooq
{

QString ObjectHookName::objectName(QObject* object)
{
	//return rawObjectName(object).toLower().replace(":", "-");
	return rawObjectName(object);
}

QString ObjectHookName::rawObjectName(QObject* object)
{
	// Grab the object name
	if(!object->objectName().isEmpty())
	{
		return object->objectName();
	}

	// If it's got no parent, classname:0
	if(!object->parent())
	{
		return QString("%1-0").arg(object->metaObject()->className());
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
	);
}

QString ObjectHookName::objectPath(QObject* object)
{
	QStringList components;
	QObject* current = object;
	while(current)
	{
		components.prepend(objectName(current));
		current = current->parent();
	}
	return components.join("::");
}

} // namespace
