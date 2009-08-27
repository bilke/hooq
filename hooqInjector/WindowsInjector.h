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
#pragma once

#include "InjectorImport.h"

#include "Injector.h"

class QTimer;

namespace Hooq
{

/// Windows-based DLL injector for Linux/OSX
class HOOQ_INJECTOR_EXPORT WindowsInjector : public Injector
{
	Q_OBJECT
	public:
		WindowsInjector(QObject* parent = 0);
		virtual ~WindowsInjector();

		virtual void startAndAttach(const QString& application);
		virtual void attach(Q_PID processId);
	private slots:
		void attachToSpawnedProcess();
	private:
		static QString libraryPath();
		QProcess* m_process;
		QTimer* m_attachTimer;
};

} // namespace
