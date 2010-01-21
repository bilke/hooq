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
#include "MainWindow.h"

#include <QApplication>

#ifdef WITH_BREAKPAD
#include "BreakpadCrashHandler.h"
#include <QDesktopServices>
#endif

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	app.setApplicationName("Hooq");
	app.setApplicationVersion("0.1");
	app.setOrganizationDomain("hooq.org");
	app.setOrganizationName("Hooq Developers");

#ifdef WITH_BREAKPAD
	BreakpadCrashHandler handler(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation) + "/hooq-crashes");
#endif

	MainWindow window;
	window.show();

	return app.exec();
}
