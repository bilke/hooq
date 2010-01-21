#pragma once
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

#include <QString>

namespace google_breakpad
{
	class ExceptionHandler;
}

/** Crash handler which installs platform-specific hooks to catch
  * application crashes and create 'minidump' files containing
  * a description of the program state.
  *
  * See http://code.google.com/p/google-breakpad/ for information on
  * interpreting the minidump files which are created.
  *
  * To use BreakpadCrashHandler, construct an instance of it early in
  * your application lifecycle (eg. in main()) which is destroyed just
  * before a normal application exit.
  */
class BreakpadCrashHandler
{
	public:
		/** Construct a crash handler and install hooks to catch
		  * application crashes.  Upon a crash, a minidump file
		  * will be created in @p outputDirectory with the name
		  * <random UUID>.dmp
		  */
		BreakpadCrashHandler(const QString& outputDirectory);
		~BreakpadCrashHandler();

		/** Returns the absolute file path of the most recent valid
		  * minidump file in @p outputDirectory
		  */
		static QString mostRecentMiniDump(const QString& outputDirectory);

		/** Removes all valid minidump files in @p outputDirectory,
		  * except the most recent one.
		  */
		static void removeOldMiniDumps(const QString& outputDirectory);


	private:
		static bool isValidMiniDump(const QString& filePath);
		// return a list of all valid minidump files in 'outputDirectory'
		// sorted by last modified time (most recent first)
		static QStringList findMiniDumps(const QString& outputDirectory);

		google_breakpad::ExceptionHandler* m_exceptionHandler;
		QString m_outputDir;
};
