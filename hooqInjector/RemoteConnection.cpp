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

void RemoteConnection::start(const QString& application, Injector* injector)
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

	injector->startAndAttach(application);
}

void RemoteConnection::acceptConnection()
{
	emit connected(m_localServer->nextPendingConnection());
}

}; // namespace
