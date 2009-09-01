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
#include "VariantMapModel.h"

#include <QApplication>
#include <QFontMetrics>
#include <QSize>
#include <QStringList>

VariantMapModel::VariantMapModel(const QVariantMap& map, QObject* parent)
: QAbstractTableModel(parent)
, m_map(map)
{
}

int VariantMapModel::columnCount(const QModelIndex& index) const
{
	Q_UNUSED(index);
	return 4;
}

int VariantMapModel::rowCount(const QModelIndex& index) const
{
	if(index.isValid())
	{
		return 0;
	}
	return m_map.count();
}

QVariant VariantMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}
	switch(section)
	{
		case 0:
			return tr("Name");
		case 1:
			return tr("Value");
		case 2:
			return tr("Fetch");
		case 3:
			return tr("Compare");
	}
	return QVariant();
}

QVariant VariantMapModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}
	const QString key = m_map.keys().at(index.row());
	switch(role)
	{
		case Qt::DisplayRole:
			switch(index.column())
			{
				case 0:
					return key;
				case 1:
					return m_map.value(key).toString();
				default:
					return QVariant();
			}
			break;
		case Qt::SizeHintRole:
		{
			QFontMetrics metrics(QApplication::fontMetrics());
			switch(index.column())
			{
				case 0:
					return metrics.size(0, key);
				case 1:
					return metrics.size(0, m_map.value(key).toString());
				default:
					return QSize(32, 32);
			}
		}
		case PropertyNameRole:
			return key;
		case PropertyValueRole:
			return m_map.value(key);
	}
	return QVariant();
}
