/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>

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
///@author Robert Knight <robert.knight@mendeley.com>
#include "BreakpadCrashHandler.h"

#if defined(Q_OS_LINUX)
  #include "google-breakpad/src/client/linux/handler/exception_handler.h"
#elif defined(Q_OS_MAC)
  #include "google-breakpad/src/client/mac/handler/exception_handler.h"
#elif defined(Q_OS_WIN)
  #include "google-breakpad/src/client/windows/handler/exception_handler.h"
#endif

#include <QDateTime>
#include <QDir>
#include <QFile>

#include <QtDebug>

BreakpadCrashHandler::BreakpadCrashHandler(const QString& outputDirectory)
: m_exceptionHandler(0)
, m_outputDir(outputDirectory)
{
	// create minidump output directory if it does not already exist
	QDir(outputDirectory).mkpath(".");

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	m_exceptionHandler = new google_breakpad::ExceptionHandler(
	   outputDirectory.toStdString(), /* minidump output directory */
	   0,   /* filter */
	   0,   /* minidump callback */
	   0,   /* callback_context */
	   true /* install_handler */
	   );
#elif defined(Q_OS_WIN)
	m_exceptionHandler = new google_breakpad::ExceptionHandler(
	   outputDirectory.toStdWString(), /* minidump output directory */
	   0,   /* filter */
	   0,   /* minidump callback */
	   0,   /* calback_context */
	   google_breakpad::ExceptionHandler::HANDLER_ALL /* handler_types */
	   );
#endif
}

BreakpadCrashHandler::~BreakpadCrashHandler()
{
	delete m_exceptionHandler;
}

void BreakpadCrashHandler::removeOldMiniDumps(const QString& outputDirectory)
{
	QStringList minidumps = findMiniDumps(outputDirectory);
	for (int i = 1; i < minidumps.count(); i++)
	{
		if (!QFile::remove(minidumps.at(i)))
		{
			qWarning() << "Unable to remove old minidump" << minidumps.at(i);
		}
		else
		{
			qDebug() << "removing old minidump" << minidumps.at(i);
		}
	}
}

QString BreakpadCrashHandler::mostRecentMiniDump(const QString& outputDirectoryPath)
{
	QStringList minidumps = findMiniDumps(outputDirectoryPath);
	if (!minidumps.isEmpty())
	{
		qDebug() << "newest minidump" << minidumps.first();
		return minidumps.first();
	}
	else
	{
		return QString();
	}
}

QStringList BreakpadCrashHandler::findMiniDumps(const QString& outputDirectoryPath)
{
	QStringList minidumpPaths;
	QDir outputDir(outputDirectoryPath);
	Q_FOREACH(const QFileInfo& fileInfo, outputDir.entryInfoList(QStringList() << "*.dmp",QDir::NoFilter,QDir::Time))
	{
		qDebug() << "checking minidump file" << fileInfo.absoluteFilePath() << fileInfo.lastModified();
		if (isValidMiniDump(fileInfo.absoluteFilePath()))
		{
			minidumpPaths.append(fileInfo.absoluteFilePath());
		}
	}
	return minidumpPaths;
}

bool BreakpadCrashHandler::isValidMiniDump(const QString& filePath)
{
	QFile dumpFile(filePath);
	if (dumpFile.open(QIODevice::ReadOnly))
	{
		QByteArray mimeMagic = dumpFile.read(4);
		if (mimeMagic == "MDMP")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
