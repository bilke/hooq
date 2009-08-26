#pragma once

#include <QObject>
#include <QString>


class QIODevice;
class QLocalServer;
class QLocalSocket;

namespace Hooq
{
class Injector;

class RemotePlayback : public QObject
{
	Q_OBJECT
	public:
		RemotePlayback(QObject* parent = 0);
		void start(const QString& application, QIODevice* logDevice, Injector* injector);
		// TODO: attach()
	private slots:
		void acceptConnection();
	private:
		QIODevice* m_log;

		QLocalSocket* m_socket;
		QLocalServer* m_localServer;
};

} // namespace
