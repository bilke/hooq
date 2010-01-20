/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>
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
#include "Event.h"

namespace Hooq
{

PathEvent::PathEvent(const QString& objectPath)
: Event()
, m_objectPath(objectPath)
{
}

bool Event::ack() const
{
	return m_ack;
}

void Event::setAck(bool ack)
{
	m_ack = ack;
}

Event::~Event()
{
}

Event::Event()
: m_ack(true)
{
}

Event* Event::withoutAck(Event* event)
{
	event->setAck(false);
	return event;
}

QString Event::tag() const
{
	return m_tag;
}

Event* Event::addTag(const QString& tag, Event* event)
{
	event->m_tag = tag;
	return event;
}

QString PathEvent::objectPath() const
{
	return m_objectPath;
}

PathEvent::~PathEvent()
{
}

ObjectEvent::~ObjectEvent()
{
}

SleepEvent::~SleepEvent()
{
}

DumpEvent::DumpEvent(const QString& objectPath)
: TypedEvent<DumpEvent, PathEvent>(objectPath)
{
}

SleepEvent::SleepEvent(int msec)
: m_msec(msec)
{
}

int SleepEvent::msec() const
{
	return m_msec;
}

ObjectEvent::ObjectEvent(const QString& objectPath, QEvent* qtEvent)
: TypedEvent<ObjectEvent,PathEvent>(objectPath)
, m_qtEvent(qtEvent)
{
}

QEvent* ObjectEvent::qtEvent() const
{
	return m_qtEvent;
}

DropEvent::DropEvent(const QString& objectPath, const QPoint& pos, const QPoint& globalPos)
: TypedEvent<DropEvent,PathEvent>(objectPath)
, m_pos(pos)
, m_globalPos(globalPos)
{
}

DropEvent::~DropEvent()
{
}

QPoint DropEvent::pos() const
{
	return m_pos;
}

QPoint DropEvent::globalPos() const
{
	return m_globalPos;
}

FocusEvent::~FocusEvent()
{
}

FocusEvent::FocusEvent(const QString& objectPath, Qt::FocusReason reason)
: TypedEvent<FocusEvent, PathEvent>(objectPath)
, m_reason(reason)
{
}

Qt::FocusReason FocusEvent::reason() const
{
	return m_reason;
}

CloseEvent::CloseEvent(const QString& objectPath)
: TypedEvent<CloseEvent, PathEvent>(objectPath)
{
}

CloseEvent::~CloseEvent()
{
}

};
