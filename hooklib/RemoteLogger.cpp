#include "RemoteLogger.h"

#include "Injector.h"

#include "../lib/Communication.h"

#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>

namespace Hooq
{

RemoteLogger::RemoteLogger(QObject* parent)
: QObject(parent)
, m_socket(0)
, m_localServer(0)
{
}

void RemoteLogger::start(const QString& application, QIODevice* logDevice, Injector* injector)
{
	Q_UNUSED(logDevice); // XXX FIXME XXX

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

	injector->startAndAttach(application, Injector::Record);
}

void RemoteLogger::logData()
{
	qDebug() << m_socket->readAll().constData();
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
}

} // namespace
