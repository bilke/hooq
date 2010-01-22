/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>

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
#include "RemoteApplicationPrototype.h"

RemoteApplicationPrototype::RemoteApplicationPrototype(QObject* parent)
: QObject(parent)
, m_closed(false)
{
}

RemoteApplicationPrototype::RemoteApplicationPrototype(const QString& path, const QStringList& arguments, QObject* parent)
: QObject(parent)
, m_closed(false)
, m_path(path)
, m_arguments(arguments)
{
}

RemoteApplicationPrototype::~RemoteApplicationPrototype()
{
	close();
}

bool RemoteApplicationPrototype::useDefaults() const
{
	return m_path.isEmpty();
}

void RemoteApplicationPrototype::close()
{
	if(!m_closed)
	{
		m_closed = true;
		emit closeRequested();
	}
}

QString RemoteApplicationPrototype::path() const
{
	Q_ASSERT(!useDefaults());
	return m_path;
}

QStringList RemoteApplicationPrototype::arguments() const
{
	Q_ASSERT(!useDefaults());
	return m_arguments;
}
