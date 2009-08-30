#include "XmlPropertyDumper.h"

#include "../common/ObjectHookName.h"

#include <QIODevice>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>

namespace Hooq
{

void XmlPropertyDumper::dump(QObject* object, QIODevice* out)
{
	const QMetaObject* metaObject = object->metaObject();

	QXmlStreamWriter xml(out);
	xml.writeStartDocument();
	xml.writeStartElement("object");
	xml.writeAttribute("path", ObjectHookName::objectPath(object));
	xml.writeAttribute("class", metaObject->className());


	for(int i = 0; i < metaObject->propertyCount(); ++i)
	{
		QMetaProperty property = metaObject->property(i);
		switch(property.type())
		{
			case QVariant::Bool:
				writeProperty(property, property.read(object).toBool() ? "true" : "false", &xml);
				break;
			case QVariant::Int:
			case QVariant::Double:
			case QVariant::String:
				writeProperty(property, property.read(object).toString(), &xml);
				break;
			default:
				break;
		};
	}

	xml.writeEndElement(); // object;
	xml.writeEndDocument();
}

void XmlPropertyDumper::writeProperty(const QMetaProperty& property, const QString& value, QXmlStreamWriter* xml)
{
	xml->writeStartElement("property");
	xml->writeAttribute("name", property.name());
	xml->writeAttribute("type", property.typeName());
	xml->writeCharacters(value);
	xml->writeEndElement(); // property
}

}; // namespace
