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
#pragma once

#include "UilibExport.h"

#include <QKeySequence>
#include <QObject>

class QAbstractItemView;
class QModelIndex;

/** Emit pressed/released signals when a certain key is pressed on a valid model index.
 *
 * For example, this lets you easily hook up a QAbstractItemView to a deleteItem(QModelIndex)
 * slot.
 */
class HOOQ_UILIB_EXPORT ModelIndexKeyEventObserver : public QObject
{
	Q_OBJECT
	public:
		ModelIndexKeyEventObserver(QKeySequence::StandardKey key, QAbstractItemView* parent);

		virtual bool eventFilter(QObject* watched, QEvent* event);
	signals:
		void pressed(const QModelIndex&);
		void released(const QModelIndex&);
	private:
		QKeySequence::StandardKey m_key;
		QAbstractItemView* m_view;
};
