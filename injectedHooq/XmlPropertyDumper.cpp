/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
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
	xml.writeAttribute("name", ObjectHookName::objectName(object));
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
