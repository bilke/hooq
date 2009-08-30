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

#include <QStyledItemDelegate>

#include <QHash>
#include <QIcon>
#include <QSet>
#include <QString>

class QStyleOptionButton;

class PushButtonDelegate: public QStyledItemDelegate
{
	Q_OBJECT;
	public:
		PushButtonDelegate(QAbstractItemView* view, QObject* parent = 0);
		~PushButtonDelegate();

		void addButton(int column, const QIcon& icon, const QString& text = QString());
		void addButton(int column, const QString& text);

		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	public slots:
		void hoverIndex(const QModelIndex& index);
		void depressIndex(const QModelIndex& index);
		void release();
	private:
		void initStyleOption(QStyleOptionButton* out, const QStyleOptionViewItem& in, const QModelIndex& index) const;

		QSet<int> m_buttonColumns;
		QHash<int, QIcon> m_buttonIcons;
		QHash<int, QString> m_buttonText;

		QPersistentModelIndex m_hoverIndex;
		QPersistentModelIndex m_pressedIndex;
};
