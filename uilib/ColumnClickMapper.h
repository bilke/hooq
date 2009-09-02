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
#pragma once

#include <QMultiHash>
#include <QObject>
#include <QPair>
#include <QPointer>

class QAbstractItemView;
class QModelIndex;

/** Map clicks on specific columns of a QAbstractItemView to different slots.
 *
 * This means you can easily, for example, map column 1 to "play", and column 2 to "edit",
 * without needing a separate dispatcher slot.
 */
class ColumnClickMapper : public QObject
{
	Q_OBJECT
	public:
		ColumnClickMapper(QAbstractItemView* parent);
		void addMapping(int column, QObject* receiever, const char* slot);
	private slots:
		void map(const QModelIndex&);
	private:
		typedef QPair<QPointer<QObject>, const char*> Target;
		QMultiHash<int, Target> m_map;
};
