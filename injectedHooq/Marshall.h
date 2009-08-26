#pragma once

#include <QObject>

class QLocalSocket;

namespace Hooq
{

class Marshall : public QObject
{
	Q_OBJECT
	public:
		Marshall();
	private slots:
		void readCommand();
	private:
		static void flushSocket();
		static QLocalSocket* m_socket;
};

} // namespace
