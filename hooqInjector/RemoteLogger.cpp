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
	m_log->write(m_socket->readAll().constData());
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
