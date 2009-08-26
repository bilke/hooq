#include "Communication.h"

#include <QCoreApplication>
#include <QFileInfo>

namespace Hooq
{

QString Communication::serverName()
{
	return serverName(QCoreApplication::applicationFilePath());
}

QString Communication::serverName(const QString& application)
{
	return QString("Hooq_%1").arg(QLatin1String(QFileInfo(application).canonicalFilePath().toLatin1().toHex()));
}

};
