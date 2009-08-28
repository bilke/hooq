#include "RemoteObjectPrototype.h"

#include <QDebug>

RemoteObjectPrototype::RemoteObjectPrototype(const QString& path, QObject* parent)
: QObject(parent)
, m_path(path)
{
}

RemoteObjectPrototype::~RemoteObjectPrototype()
{
}

void RemoteObjectPrototype::moveMouse(const QVariantMap& parameters)
{
	qDebug() << Q_FUNC_INFO << parameters;
}

void RemoteObjectPrototype::pressMouseButton(const QVariantMap& parameters)
{
	qDebug() << Q_FUNC_INFO << parameters;
}

void RemoteObjectPrototype::releaseMouseButton(const QVariantMap& parameters)
{
	qDebug() << Q_FUNC_INFO << parameters;
}

void RemoteObjectPrototype::pressKey(const QVariantMap& parameters)
{
	qDebug() << Q_FUNC_INFO << parameters;
}

void RemoteObjectPrototype::releaseKey(const QVariantMap& parameters)
{
	qDebug() << Q_FUNC_INFO << parameters;
}

void RemoteObjectPrototype::mouseWheel(const QVariantMap& parameters)
{
	qDebug() << Q_FUNC_INFO << parameters;
}
