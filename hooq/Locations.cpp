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
#include "Locations.h"

#include <QDir>
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
	return QDir::cleanPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tests");
}

QString Locations::scriptPath(const QString& testSet, const QString& testName)
{

	return QDir::cleanPath(QString("%1/%2.qs").arg(testSetLocation(testSet)).arg(testName));
}

QString Locations::testSetLocation(const QString& testSet)
{
	if(testSet.isEmpty())
	{
		return QString();
	}

	return QDir::cleanPath(testSetsLocation() + QString("/%1/").arg(testSet));
}
