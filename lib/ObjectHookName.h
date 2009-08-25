#pragma once

#include <QString>

class QObject;

class ObjectHookName
{
	public:
		static QString objectPath(QObject* object);
		static QString objectName(QObject* object);
};
