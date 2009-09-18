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
#include "TestSetBackup.h"

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

QTextStream err(stderr);
QTextStream out(stdout);

void printHelp()
{
	err << "Syntax: " << qApp->arguments().at(0) << " OPERATION FILE" << endl;
	err << "Operations:" << endl;
	err << "\tidentify\tidentify the test set in FILE" << endl;
	err << "\tlist    \tlist all the tests in FILE" << endl;
	err << "\textract \textract the contents of FILE" << endl;
	err << "\tcreate  \tcreate FILE.hqt from the contents of FILE/" << endl;
}

// return codes:
// 0: all good
// 1: invalid arguments
// 2: file not found
// 3: not hooq backup file

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);


	if(app.arguments().count() != 3)
	{
		printHelp();
		return 1;
	}

	const QString operation = app.arguments().at(1);
	const QString file = app.arguments().at(2);

	if(!QFile::exists(file))
	{
		err << "File does not exist." << endl;
		return 2;
	}

	if(operation == "identify")
	{
		const QString testSet = TestSetBackup::identify(file);
		if(testSet.isEmpty())
		{
			err << "File '" << file << "' is not a valid hooq backup file." << endl;
			return 3;
		}
		out << "File '" << file << "' contains test set '" << testSet << "'" << endl;
		return 0;
	}

	if(operation == "list")
	{
		const QString testSet = TestSetBackup::identify(file);
		if(testSet.isEmpty())
		{
			err << "File '" << file << "' is not a valid hooq backup file." << endl;
			return 3;
		}
		const QStringList tests = TestSetBackup::list(file);
		Q_FOREACH(QString test, TestSetBackup::list(file))
		{
			test.chop(3);
			out << test << endl;
		}
		return 0;
	}

	if(operation == "extract")
	{
		const QString testSet = TestSetBackup::restore(file, ".");
		if(testSet.isEmpty())
		{
			err << "File '" << file << "' is not a valid hooq backup file." << endl;
			return 3;
		}
		out << "Extracted test set to " << testSet << "/" << endl;
		return 0;
	}

	if(operation == "create")
	{
		const QString outFile = file + ".hqt";
		if(TestSetBackup::backup(file, outFile, "."))
		{
			out << "Created " << outFile << endl;
			return 0;
		}
		err << "Could not create backup." << endl;
		return 3;
	}

	err << "Unknown command '" << operation << "'" << endl;
	return 1;
}
