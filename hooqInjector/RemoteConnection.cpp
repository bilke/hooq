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

#include <QDir>
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>

namespace Hooq
{

RemoteConnectionException::RemoteConnectionException(QAbstractSocket::SocketError serverError, const QString& errorString)
: m_serverError(serverError)
, m_errorString(errorString)
{
}

QAbstractSocket::SocketError RemoteConnectionException::serverError() const
{
	return m_serverError;
}

QString RemoteConnectionException::errorString() const
{
	return m_errorString;
}

RemoteConnection::RemoteConnection(QObject* parent)
: QObject(parent)
, m_localServer(0)
{
}

void RemoteConnection::start(const QString& application, const QStringList& arguments, Injector* injector)
{
	delete m_localServer;

	m_localServer = new QTcpServer(this);
	connect(
		m_localServer,
		SIGNAL(newConnection()),
		SLOT(acceptConnection())
	);

	if(!m_localServer->listen(QHostAddress::LocalHost, Communication::serverPort()))
	{
		throw RemoteConnectionException(m_localServer->serverError(), m_localServer->errorString());
	}

	injector->startAndAttach(application, arguments);
}

void RemoteConnection::acceptConnection()
{
	QTcpSocket* socket = m_localServer->nextPendingConnection();
	m_localServer->close();
	emit connected(socket);
}

}; // namespace
