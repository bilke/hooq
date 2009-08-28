#include "EnumConverter.h"

#include <QMetaEnum>
#include <QMetaObject>
#include <QObject>

class QtMetaObjectFetcher : private QObject
{
	public:
		static QMetaObject fetch()
		{
			return staticQtMetaObject;
		}
};

QString EnumConverter::stringFromValue(int value, const char* enumName)
{
	return stringFromValue(value, enumName, QtMetaObjectFetcher::fetch());
}

QString EnumConverter::stringFromValue(int value, const char* enumName, const QMetaObject& object)
{
	const int enumIndex = object.indexOfEnumerator(enumName);
	Q_ASSERT(enumIndex >= 0);
	const char* name = object.enumerator(enumIndex).valueToKey(value);
	Q_ASSERT(name);
	return QString::fromLatin1(name);
}
