#pragma once

#include "InjectorImport.h"

#include <QObject>

class QLocalServer;
class QLocalSocket;

namespace Hooq
{
class Injector;

class HOOQ_INJECTOR_EXPORT RemoteConnection : public QObject
{
	Q_OBJECT;
	public:
		RemoteConnection(QObject* parent);
		void start(const QString& application, Injector* injector);
	signals:
		void connected(QLocalSocket* socket);
	private slots:
		void acceptConnection();
	private:
		QLocalServer* m_localServer;
};

} // namespace
