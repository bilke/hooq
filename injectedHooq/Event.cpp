/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
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

DumpEvent::DumpEvent(const QString& objectPath)
: m_objectPath(objectPath)
{
}

Event::Type DumpEvent::type() const
{
	return Dump;
}

QString DumpEvent::objectPath() const
{
	return m_objectPath;
}

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

PickEvent::PickEvent()
{
}

Event::Type PickEvent::type() const
{
	return Pick;
}

};
