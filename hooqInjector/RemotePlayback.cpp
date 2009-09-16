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
#include "RemotePlayback.h"

#include "RemoteConnection.h"

#include <QDebug>
#include <QLocalSocket>

namespace Hooq
{

RemotePlayback::RemotePlayback(QObject* parent)
: QObject(parent)
, m_log(0)
, m_server(new RemoteConnection(this))
, m_socket(0)
{
	connect(
		m_server,
		SIGNAL(connected(QLocalSocket*)),
		SLOT(startPlayback(QLocalSocket*))
	);
}

void RemotePlayback::start(const QString& application, const QStringList& arguments, QIODevice* logDevice, Injector* injector)
{
	Q_ASSERT(logDevice && logDevice->isOpen() && logDevice->isReadable());
	m_log = logDevice;

	m_server->start(application, arguments, injector);
}

void RemotePlayback::startPlayback(QLocalSocket* socket)
{
	delete m_socket;
	m_socket = socket;
	Q_ASSERT(m_socket->state() == QLocalSocket::ConnectedState && m_socket->isWritable() && m_socket->isOpen());
	m_socket->write("PLAY\n");
	m_socket->write(m_log->readAll());
	m_socket->flush();
}

} // namespace
