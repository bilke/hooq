#include "Marshall.h"

#include "Logger.h"
#include "Player.h"

#include "../common/Communication.h"

#include <QCoreApplication>
#include <QLocalSocket>

namespace Hooq
{

QLocalSocket* Marshall::m_socket;

Marshall::Marshall()
: QObject()
{
	m_socket = new QLocalSocket();
	m_socket->connectToServer(Communication::serverName());
	m_socket->waitForConnected(1000);
	Q_ASSERT(m_socket->state() == QLocalSocket::ConnectedState && m_socket->isReadable());
	connect(
		m_socket,
		SIGNAL(readyRead()),
		SLOT(readCommand())
	);
	qAddPostRoutine(flushSocket);
}

void Marshall::flushSocket()
{
	m_socket->flush();
	m_socket->close();
}

void Marshall::readCommand()
{
	disconnect(m_socket, 0, this, 0);
	const QByteArray command = m_socket->readLine();
	if(command == "RECORD\n")
	{
		Logger::instance(m_socket);
	}
	else if(command == "PLAY\n")
	{
		new Player(m_socket);
	}
	else
	{
		qFatal("Unknown command: %s", command);
	}
}

} // namespace
