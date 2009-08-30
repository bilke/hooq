#pragma once

#include <QString>
#include <QVariant>

class QIODevice;

class ObjectInformation
{
	public:
		ObjectInformation(const QString& objectName, const QString& objectPath, const QVariantMap& properties, const QString& className);
		QString className() const;
		QString name() const;
		QString path() const;
		QVariantMap properties() const;

		static ObjectInformation fromXml(QIODevice* source);
	private:
		QString m_className;
		QString m_name;
		QString m_path;
		QVariantMap m_properties;
};
