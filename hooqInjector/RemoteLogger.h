#pragma once

#include "InjectorImport.h"

#include <QObject>
#include <QString>


class QIODevice;
class QLocalServer;
class QLocalSocket;

namespace Hooq
{
class Injector;

class HOOQ_INJECTOR_EXPORT RemoteLogger : public QObject
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
		QIODevice* m_log;

		QLocalSocket* m_socket;
		QLocalServer* m_localServer;
};

} // namespace
