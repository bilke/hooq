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
	if(testSet.isEmpty())
	{
		return;
	}

	QSettings settings;
	settings.beginGroup(testSet);
	settings.setValue("application", application);
}

QString Locations::testSetsLocation()
{
	return QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tests";
}

QString Locations::scriptPath(const QString& testSet, const QString& testName)
{
	return QString("%1/%2.qs").arg(testSetLocation(testSet)).arg(testName);
}

QString Locations::testSetLocation(const QString& testSet)
{
	if(testSet.isEmpty())
	{
		return QString();
	}

	return testSetsLocation() + QString("/%1/").arg(testSet);
}
