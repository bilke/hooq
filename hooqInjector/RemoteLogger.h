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

#include <QFlags>
#include <QObject>
#include <QString>


class QIODevice;
class QLocalSocket;

namespace Hooq
{
class Injector;
class RemoteConnection;

class HOOQ_INJECTOR_EXPORT RemoteLogger : public QObject
{
	Q_OBJECT
	public:
		enum Event
		{
			NoEvents = 0,
			MouseMoveEvent = 1 << 0,
			KeyPressEvent = 1 << 1,
			KeyReleaseEvent = 1 << 2,
			MouseButtonPressEvent = 1 << 3,
			MouseButtonReleaseEvent = 1 << 4,
			MouseWheelEvent = 1 << 5,
			AllEvents = ~NoEvents
		};
		Q_DECLARE_FLAGS(Events, Event);
		// At the moment, only the MouseMoveEvent flag does any difference
		RemoteLogger(QObject* parent = 0); // AllEvents
		RemoteLogger(Events events, QObject* parent = 0);

		void start(const QString& application, QIODevice* logDevice, Injector* injector);
		// TODO: attach()
	private slots:
		void logData();
		void startLogging(QLocalSocket* socket);
	private:
		Events m_events;
		QIODevice* m_log;
		RemoteConnection* m_server;
		QLocalSocket* m_socket;
};

} // namespace

Q_DECLARE_OPERATORS_FOR_FLAGS(Hooq::RemoteLogger::Events);
