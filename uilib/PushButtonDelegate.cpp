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
#include "PushButtonDelegate.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <QPainter>

PushButtonDelegate::PushButtonDelegate(QAbstractItemView* view, QObject* parent)
: QStyledItemDelegate(parent)
, m_enabledRole(-1)
{
	view->setMouseTracking(true);
	// All of these connections are required to get the fake push buttons redrawn every time they need to be, and drawn in the proper state
	connect(
		view,
		SIGNAL(pressed(QModelIndex)),
		SLOT(depressIndex(QModelIndex))
	);
	connect(
		view,
		SIGNAL(pressed(QModelIndex)),
		view,
		SLOT(update(QModelIndex))
	);
	connect(
		view,
		SIGNAL(entered(QModelIndex)),
		this,
		SLOT(hoverIndex(QModelIndex))
	);
	connect(
		view,
		SIGNAL(clicked(QModelIndex)),
		this,
		SLOT(release())
	);
	connect(
		view,
		SIGNAL(clicked(QModelIndex)),
		view,
		SLOT(update(QModelIndex))
	);
}

int PushButtonDelegate::enabledRole() const
{
	return m_enabledRole;
}

void PushButtonDelegate::setEnabledRole(int enabledRole)
{
	m_enabledRole = enabledRole;
}

void PushButtonDelegate::addButton(int column, const QIcon& icon, const QString& text)
{
	m_buttonColumns.insert(column);
	m_buttonIcons[column] = icon;
	m_buttonText[column] = text;
}

void PushButtonDelegate::addButton(int column, const QString& text)
{
	addButton(column, QIcon(), text);
}

void PushButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	if(index.isValid())
	{
		if(m_buttonColumns.contains(index.column()))
		{
			QStyleOptionButton opt;
			initStyleOptionButton(&opt, option, index);
			QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
		}
	}
}

void PushButtonDelegate::release()
{
	m_pressedIndex = QModelIndex();
}

void PushButtonDelegate::depressIndex(const QModelIndex& index)
{
	m_pressedIndex = index;
}

void PushButtonDelegate::hoverIndex(const QModelIndex& index)
{
	release();
	m_hoverIndex = index;
}

void PushButtonDelegate::initStyleOptionButton(QStyleOptionButton* out, const QStyleOptionViewItem& in, const QModelIndex& index) const
{
	out->icon = m_buttonIcons.value(index.column());
	out->text= m_buttonText.value(index.column());
	out->iconSize = QSize(16, 16);
	out->rect = in.rect;
	out->state = QStyle::State_None;

	bool enabled;
	if(enabledRole() == -1)
	{
		enabled = true;
	}
	else
	{
		enabled = index.data(enabledRole()).toBool();
	}

	if(enabled)
	{
		out->state |= in.state & QStyle::State_Enabled;
	}
	else
	{
		out->palette.setCurrentColorGroup(QPalette::Disabled);
	}

	if(enabled && index == m_pressedIndex && in.state & QStyle::State_MouseOver)
	{
		out->state |= QStyle::State_Sunken;
	}
	if(index == m_hoverIndex && in.state & QStyle::State_MouseOver)
	{
		out->state |= QStyle::State_MouseOver;
	}
}

PushButtonDelegate::~PushButtonDelegate()
{
}
