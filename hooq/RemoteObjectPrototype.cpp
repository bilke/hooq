#include "RemoteObjectPrototype.h"

#include <QDebug>
#include <QPoint>

Q_DECLARE_METATYPE(Qt::MouseButton);
Q_DECLARE_METATYPE(Qt::MouseButtons);
Q_DECLARE_METATYPE(Qt::Orientation);
Q_DECLARE_METATYPE(Qt::KeyboardModifiers);

RemoteObjectPrototype::RemoteObjectPrototype(const QString& path, QObject* parent)
: QObject(parent)
, m_path(path)
{
}

RemoteObjectPrototype::~RemoteObjectPrototype()
{
}

QString RemoteObjectPrototype::path() const
{
	return m_path;
}

void RemoteObjectPrototype::moveMouse(const QVariantMap& parameters)
{
	emit mouseMoveEvent(
		path(),
		QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		),
		parameters.value("button").value<Qt::MouseButton>(),
		parameters.value("buttons").value<Qt::MouseButtons>(),
		parameters.value("modifiers").value<Qt::KeyboardModifiers>()
	);
}

void RemoteObjectPrototype::pressMouseButton(const QVariantMap& parameters)
{
	emit mousePressEvent(
		path(),
		QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		),
		parameters.value("button").value<Qt::MouseButton>(),
		parameters.value("buttons").value<Qt::MouseButtons>(),
		parameters.value("modifiers").value<Qt::KeyboardModifiers>()
	);
}

void RemoteObjectPrototype::releaseMouseButton(const QVariantMap& parameters)
{
	emit mouseReleaseEvent(
		path(),
		QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		),
		parameters.value("button").value<Qt::MouseButton>(),
		parameters.value("buttons").value<Qt::MouseButtons>(),
		parameters.value("modifiers").value<Qt::KeyboardModifiers>()
	);
}

void RemoteObjectPrototype::pressKey(const QVariantMap& parameters)
{
}

void RemoteObjectPrototype::releaseKey(const QVariantMap& parameters)
{
}

void RemoteObjectPrototype::mouseWheel(const QVariantMap& parameters)
{
	emit wheelEvent(
		path(),
		QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		),
		parameters.value("delta").toInt(),
		parameters.value("buttons").value<Qt::MouseButtons>(),
		parameters.value("modifiers").value<Qt::KeyboardModifiers>(),
		parameters.value("orientation").value<Qt::Orientation>()
	);
}
