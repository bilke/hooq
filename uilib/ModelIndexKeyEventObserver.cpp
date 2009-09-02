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
#include "ModelIndexKeyEventObserver.h"

#include <QAbstractItemView>
#include <QKeyEvent>

ModelIndexKeyEventObserver::ModelIndexKeyEventObserver(QKeySequence::StandardKey key, QAbstractItemView* parent)
: QObject(parent)
, m_key(key)
, m_view(parent)
{
	m_view->installEventFilter(this);
}

bool ModelIndexKeyEventObserver::eventFilter(QObject* watched, QEvent* event)
{
	Q_ASSERT(watched == m_view);
	if(watched == m_view)
	{
		const bool isKeyPress = event->type() == QEvent::KeyPress;
		const bool isKeyRelease = event->type() == QEvent::KeyRelease;
		if(isKeyPress || isKeyRelease)
		{
			QKeyEvent* k = static_cast<QKeyEvent*>(event);
			const QModelIndex& index = m_view->currentIndex();
			if(k->matches(m_key) & index.isValid())
			{
				if(isKeyPress)
				{
					emit pressed(index);
				}
				else
				{
					emit released(index);
				}
			}
		}
	}
	return false;
}
