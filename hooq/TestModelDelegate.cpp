#include "TestModelDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QStyle>
#include <QPainter>

TestModelDelegate::TestModelDelegate(QObject* parent)
: QStyledItemDelegate(parent)
, m_runIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay))
{
//	m_editButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
}

void TestModelDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	if(index.isValid())
	{
		if(index.column() == 1)
		{
			QStyleOptionButton opt;
			initStyleOption(&opt, option, m_runIcon, index);
			QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
		}
		else if(index.column() == 2)
		{
//			m_editButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
		}
	}
}

void TestModelDelegate::release()
{
	m_pressedIndex = QModelIndex();
}

void TestModelDelegate::depressIndex(const QModelIndex& index)
{
	m_pressedIndex = index;
}

void TestModelDelegate::hoverIndex(const QModelIndex& index)
{
	release();
	m_hoverIndex = index;
}

void TestModelDelegate::initStyleOption(QStyleOptionButton* out, const QStyleOptionViewItem& in, const QIcon& icon, const QModelIndex& index) const
{
	out->icon = icon;
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

TestModelDelegate::~TestModelDelegate()
{
}
