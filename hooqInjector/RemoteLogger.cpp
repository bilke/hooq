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
#include "RemoteLogger.h"

#include "Injector.h"

#include "../common/Communication.h"

#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>

namespace Hooq
{

RemoteLogger::RemoteLogger(QObject* parent)
: QObject(parent)
, m_log(0)
, m_socket(0)
, m_localServer(0)
{
}

void RemoteLogger::start(const QString& application, QIODevice* logDevice, Injector* injector)
{
	Q_ASSERT(logDevice && logDevice->isOpen() && logDevice->isWritable());
	m_log = logDevice;

	const QString socketName = Communication::serverName(application);

	delete m_localServer;
	delete m_socket;

	m_localServer = new QLocalServer(this);
	connect(
		m_localServer,
		SIGNAL(newConnection()),
		SLOT(acceptConnection())
	);

	QLocalServer::removeServer(socketName);
	m_localServer->listen(socketName);

	injector->startAndAttach(application);
}

void RemoteLogger::logData()
{
	const QByteArray data = m_socket->readAll();
	m_log->write(data);
}

void RemoteLogger::acceptConnection()
{
	delete m_socket;
	m_socket = m_localServer->nextPendingConnection();
	connect(
		m_socket,
		SIGNAL(readyRead()),
		SLOT(logData())
	);
	m_socket->write("RECORD\n");
}

} // namespace
