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
#include "RemoteConnection.h"

#include "Injector.h"

#include "../common/Communication.h"

#include <QLocalServer>
#include <QLocalSocket>

namespace Hooq
{

RemoteConnection::RemoteConnection(QObject* parent)
: QObject(parent)
, m_localServer(0)
{
}

void RemoteConnection::start(const QString& application, const QStringList& arguments, Injector* injector)
{
	const QString socketName = Communication::serverName(application);

	delete m_localServer;

	m_localServer = new QLocalServer(this);
	connect(
		m_localServer,
		SIGNAL(newConnection()),
		SLOT(acceptConnection())
	);

	QLocalServer::removeServer(socketName);
	m_localServer->listen(socketName);

	injector->startAndAttach(application, arguments);
}

void RemoteConnection::acceptConnection()
{
	emit connected(m_localServer->nextPendingConnection());
}

}; // namespace
