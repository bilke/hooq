#pragma once

#include <QObject>
#include <QString>


class QIODevice;
class QLocalServer;
class QLocalSocket;

namespace Hooq
{
class Injector;

class RemoteLogger : public QObject
{
	Q_OBJECT
	public:
		RemoteLogger(QObject* parent = 0);
		void start(const QString& application, QIODevice* logDevice, Injector* injector);
		// TODO: attach()
	private slots:
		void logData();
		void acceptConnection();
	private:
		QLocalSocket* m_socket;
		QLocalServer* m_localServer;
};

} // namespace
