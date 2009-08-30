#pragma once

#include <QXmlStreamWriter>

class QIODevice;
class QMetaProperty;
class QObject;
class QString;

namespace Hooq
{
	class XmlPropertyDumper
	{
		public:
			static void dump(QObject* object, QIODevice* out);
		private:
			static void writeProperty(const QMetaProperty& property, const QString& value, QXmlStreamWriter* writer);
	};
};
