#pragma once

#include <QString>

class QMetaObject;

class EnumConverter
{
	public:
		static QString stringFromValue(int value, const char* enumName); // Qt global meta object
		static QString stringFromValue(int value, const char* enumName, const QMetaObject& object);
};
