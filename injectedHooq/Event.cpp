#include "Event.h"

namespace Hooq
{

SleepEvent::SleepEvent(int msec)
: m_msec(msec)
{
}

int SleepEvent::msec() const
{
	return m_msec;
}

Event::Type SleepEvent::type() const
{
	return Sleep;
}

ObjectEvent::ObjectEvent(const QString& objectPath, QEvent* qtEvent)
: m_objectPath(objectPath)
, m_qtEvent(qtEvent)
{
}

QString ObjectEvent::objectPath() const
{
	return m_objectPath;
}

QEvent* ObjectEvent::qtEvent() const
{
	return m_qtEvent;
}

Event::Type ObjectEvent::type() const
{
	return Object;
}

};
