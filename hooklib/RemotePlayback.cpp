#include "RemotePlayback.h"

#include "Injector.h"

#include "../lib/Communication.h"

#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>

namespace Hooq
{

RemotePlayback::RemotePlayback(QObject* parent)
: QObject(parent)
, m_log(0)
, m_socket(0)
, m_localServer(0)
{
}

void RemotePlayback::start(const QString& application, QIODevice* logDevice, Injector* injector)
{
	Q_ASSERT(logDevice && logDevice->isOpen() && logDevice->isReadable());
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

	injector->startAndAttach(application, Injector::Replay);
}

void RemotePlayback::acceptConnection()
{
	delete m_socket;
	m_socket = m_localServer->nextPendingConnection();
	Q_ASSERT(m_socket->state() == QLocalSocket::ConnectedState && m_socket->isWritable() && m_socket->isOpen());
	m_socket->write(m_log->readAll());
	m_socket->flush();
}

} // namespace
