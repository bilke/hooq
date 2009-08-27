#include "Locations.h"

#include <QDesktopServices>
#include <QSettings>
#include <QString>

QString Locations::applicationPath(const QString& testSet)
{
	QSettings settings;
	settings.beginGroup(testSet);
	return settings.value("application").toString();
}

void Locations::setApplicationPath(const QString& testSet, const QString& application)
{
	QSettings settings;
	settings.beginGroup(testSet);
	settings.setValue("application", application);
}

QString Locations::testSetsLocation()
{
	return QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tests";
}

QString Locations::testSetLocation(const QString& testSet)
{
	if(testSet.isEmpty())
	{
		return QString();
	}

	return testSetsLocation() + QString("/%1/").arg(testSet);
}
