#pragma once

#include <QStyledItemDelegate>

class QToolButton;

class TestModelDelegate: public QStyledItemDelegate
{
	Q_OBJECT;
	public:
		TestModelDelegate(QObject* parent = 0);
		~TestModelDelegate();

		virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	private:
		QToolButton* m_runButton;
		QToolButton* m_editButton;
};
