#pragma once

#include <QString>

class QObject;

namespace Hooq
{
	class ObjectHookName
	{
		public:
			static QString objectPath(QObject* object);
			static QString objectName(QObject* object);
		private:
			static QString rawObjectName(QObject* object);
	};
}
