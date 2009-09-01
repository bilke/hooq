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
#include "ObjectInformation.h"

#include <QDebug>
#include <QIODevice>
#include <QXmlStreamReader>

ObjectInformation::ObjectInformation(const QString& objectName, const QString& objectPath, const QVariantMap& properties, const QString& className)
: m_className(className)
, m_name(objectName)
, m_path(objectPath)
, m_properties(properties)
{
}

QString ObjectInformation::className() const
{
	return m_className;
}

QString ObjectInformation::name() const
{
	return m_name;
}

QString ObjectInformation::path() const
{
	return m_path;
}

QVariantMap ObjectInformation::properties() const
{
	return m_properties;
}

ObjectInformation ObjectInformation::fromXml(QIODevice* source)
{
	Q_ASSERT(source->isOpen() && source->isReadable());
	QXmlStreamReader xml;
	xml.setDevice(source);

	QString name;
	QString path;
	QString className;
	QVariantMap properties;

	while(!xml.atEnd())
	{
		xml.readNext();
		if(xml.tokenType() == QXmlStreamReader::StartElement)
		{
			if(xml.name() == "object")
			{
				name = xml.attributes().value("name").toString();
				path = xml.attributes().value("path").toString();
				className = xml.attributes().value("class").toString();
				continue;
			}
			if(xml.name() == "property")
			{
				const QString property = xml.attributes().value("name").toString();
				const QString typeName = xml.attributes().value("type").toString();
				const QString value = xml.readElementText();
				if(typeName == "bool")
				{
					properties.insert(property, value == "true");
					continue;
				}
				if(typeName == "QString")
				{
					properties.insert(property, value);
					continue;
				}
				if(typeName == "int")
				{
					properties.insert(property, value.toInt());
					continue;
				}
				if(typeName == "double")
				{
					properties.insert(property, value.toDouble());
					continue;
				}
				qDebug() << "Unknown property typename:" << typeName;
				continue;
			}
			// skip
			xml.readElementText();
		}
	}
	return ObjectInformation(name, path, properties, className);
}
