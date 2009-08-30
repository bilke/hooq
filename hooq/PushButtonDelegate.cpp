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
#include "PushButtonDelegate.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <QPainter>

PushButtonDelegate::PushButtonDelegate(QAbstractItemView* view, QObject* parent)
: QStyledItemDelegate(parent)
{
	addButton(1, QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
	addButton(2, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
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
			initStyleOption(&opt, option, index);
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

void PushButtonDelegate::initStyleOption(QStyleOptionButton* out, const QStyleOptionViewItem& in, const QModelIndex& index) const
{
	out->icon = m_buttonIcons.value(index.column());
	out->text= m_buttonText.value(index.column());
	out->iconSize = QSize(16, 16);
	out->rect = in.rect;
	out->state |= in.state & QStyle::State_Enabled;

	if(index == m_pressedIndex && in.state & QStyle::State_MouseOver)
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
