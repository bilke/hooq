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

#include <QStyledItemDelegate>

#include <QHash>
#include <QIcon>
#include <QSet>
#include <QString>

class QStyleOptionButton;

/** Render pushbuttons in certain columns.
 * You might like to look at ColumnClickMapper for dealing with the final click.
 *
 * This doesn't create actual push buttons, it just draws them with QApplication::style().
 *
 * You can add buttons to columns with addButton(), and optionally specifiy a role that controls
 * the enabled state of the buttons via setEnabledRole() - if enabledRole() is -1, all buttons
 * will always be enabled.
 */
class PushButtonDelegate: public QStyledItemDelegate
{
	Q_OBJECT;
	public:
		PushButtonDelegate(QAbstractItemView* view, QObject* parent = 0);
		~PushButtonDelegate();

		void addButton(int column, const QIcon& icon, const QString& text = QString());
		void addButton(int column, const QString& text);

		int enabledRole() const;
		void setEnabledRole(int);

		// QStyledItemDelegate
		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	private slots:
		void hoverIndex(const QModelIndex& index);
		void depressIndex(const QModelIndex& index);
		void release();
	private:
		void initStyleOptionButton(QStyleOptionButton* out, const QStyleOptionViewItem& in, const QModelIndex& index) const;

		QSet<int> m_buttonColumns;
		QHash<int, QIcon> m_buttonIcons;
		QHash<int, QString> m_buttonText;

		int m_enabledRole;

		QPersistentModelIndex m_hoverIndex;
		QPersistentModelIndex m_pressedIndex;
};
