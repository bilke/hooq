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
				qDebug() << "Unknown property typename:" << typeName;
				continue;
			}
			// skip
			xml.readElementText();
		}
	}
	return ObjectInformation(name, path, properties, className);
}
