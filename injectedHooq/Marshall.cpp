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
#include "Marshall.h"

#include "Logger.h"
#include "Player.h"

#include "../common/Communication.h"

#include <QCoreApplication>
#include <QLocalSocket>

#include <QFile>
#include <QTextStream>

namespace Hooq
{


QFile debugFile;
QTextStream debugStream;
Marshall* Marshall::m_instance = 0;

Marshall* Marshall::instance()
{
	if(!m_instance)
	{
		m_instance = new Marshall();
	}
	return m_instance;
}

void logMessage(QtMsgType type, const char* message)
{
	switch(type)
	{
		case QtDebugMsg:
			debugStream << "DEBUG: " << message << endl;
			break;
		case QtWarningMsg:
			debugStream << "WARNING: " << message << endl;
			break;
		case QtCriticalMsg:
			debugStream << "CRITICAL: " << message << endl;
			break;
		case QtFatalMsg:
			debugStream << "FATAL: " << message << endl;
			abort();
	}
}

QLocalSocket* Marshall::m_socket;

Marshall::Marshall()
: QObject()
{
	debugFile.setFileName("hookedDebugLog.txt");
	debugFile.open(QIODevice::WriteOnly | QIODevice::Unbuffered | QFile::Truncate);
	debugStream.setDevice(&debugFile);
	qInstallMsgHandler(logMessage);

	qDebug() << QCoreApplication::applicationFilePath();
	qDebug() << __FILE__ << __LINE__;
	m_socket = new QLocalSocket();
	m_socket->connectToServer(Communication::serverName());
	qDebug() << __FILE__ << __LINE__;
	m_socket->waitForConnected(1000);
	qDebug() << __FILE__ << __LINE__;
	Q_ASSERT(m_socket->state() == QLocalSocket::ConnectedState && m_socket->isReadable());
	qDebug() << __FILE__ << __LINE__;
	connect(
		m_socket,
		SIGNAL(readyRead()),
		SLOT(readCommand())
	);
	qDebug() << __FILE__ << __LINE__;
	qAddPostRoutine(flushSocket);
	qDebug() << __FILE__ << __LINE__;
}

void Marshall::flushSocket()
{
	qDebug() << Q_FUNC_INFO;
	m_socket->flush();
	m_socket->waitForBytesWritten(1000);
	m_socket->close();
	m_socket->waitForDisconnected(1000);
	delete instance();
	delete m_socket;
}

Marshall::~Marshall()
{
	qDebug() << Q_FUNC_INFO;
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
		qFatal("Unknown command: %s", command.constData());
	}
}

} // namespace
