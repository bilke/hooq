#pragma once

#include <QString>

class Locations
{
	public:
		static QString testSetsLocation();
		static QString testSetLocation(const QString& testSet);

		static QString applicationPath(const QString& testSet);
		static void setApplicationPath(const QString& testSet, const QString& application);

		static QString scriptPath(const QString& testSet, const QString& testName);
};
