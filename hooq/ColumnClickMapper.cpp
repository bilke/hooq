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
#include "ColumnClickMapper.h"

#include <QAbstractItemView>
#include <QMetaObject>
#include <QModelIndex>

ColumnClickMapper::ColumnClickMapper(QAbstractItemView* parent)
: QObject(parent)
{
	connect(
		parent,
		SIGNAL(clicked(QModelIndex)),
		SLOT(map(QModelIndex))
	);
}

void ColumnClickMapper::addMapping(int column, QObject* receiver, const char* slot)
{
	Q_ASSERT(column >= 0);
	Q_ASSERT(QMetaObject::checkConnectArgs("void foo(QModelIndex)", slot));
	m_map.insert(column, Target(receiver, slot));
}

void ColumnClickMapper::map(const QModelIndex& index)
{
	if(index.isValid() && m_map.contains(index.column()))
	{
		Q_FOREACH(const Target& target, m_map.values(index.column()))
		{
			if(!target.first)
			{
				continue;
			}
			// Qt internal hackery
			QString method = QString::fromLatin1(target.second);
			method.remove(0, 1); // chop off the QSLOT_CODE
			method.remove(method.indexOf('('), method.length()); // chop off the arguments
			QMetaObject::invokeMethod(target.first, method.toLatin1().constData(), Q_ARG(QModelIndex, index));
		}
	}
}
