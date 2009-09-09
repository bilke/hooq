/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "ColumnClickMapper.h"

#include <QAbstractItemView>
#include <QMetaObject>
#include <QModelIndex>
#include <QPointer>

struct ColumnClickMapper::Target
{
	Target(QObject* _receiver, const char* _slot, int _enabledRole)
	{
		receiver = _receiver;
		slot = _slot;
		enabledRole = _enabledRole;
	}

	QPointer<QObject> receiver;
	const char* slot;
	int enabledRole;
};

ColumnClickMapper::ColumnClickMapper(QAbstractItemView* parent)
: QObject(parent)
{
	connect(
		parent,
		SIGNAL(clicked(QModelIndex)),
		SLOT(map(QModelIndex))
	);
}

ColumnClickMapper::~ColumnClickMapper()
{

}

void ColumnClickMapper::addMapping(int column, QObject* receiver, const char* slot, int enabledRole)
{
	Q_ASSERT(column >= 0);
	Q_ASSERT(QMetaObject::checkConnectArgs("void foo(QModelIndex)", slot));
	m_map.insert(column, Target(receiver, slot, enabledRole));
}

void ColumnClickMapper::map(const QModelIndex& index)
{
	if(index.isValid() && m_map.contains(index.column()))
	{
		Q_FOREACH(const Target& target, m_map.values(index.column()))
		{
			if(!target.receiver)
			{
				continue;
			}
			if(target.enabledRole != -1)
			{
				if(!index.data(target.enabledRole).toBool())
				{
					continue;
				}
			}
			// Qt internal hackery
			QString method = QString::fromLatin1(target.slot);
			method.remove(0, 1); // chop off the QSLOT_CODE
			method.remove(method.indexOf('('), method.length()); // chop off the arguments
			QMetaObject::invokeMethod(target.receiver, method.toLatin1().constData(), Q_ARG(QModelIndex, index));
		}
	}
}
