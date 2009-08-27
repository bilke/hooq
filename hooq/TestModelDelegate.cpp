#include "TestModelDelegate.h"

#include <QApplication>
#include <QStyle>
#include <QToolButton>

TestModelDelegate::TestModelDelegate(QObject* parent)
: QStyledItemDelegate(parent)
, m_runButton(new QToolButton())
, m_editButton(new QToolButton())
{
	m_runButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
	m_editButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
}

void TestModelDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
	if(index.isValid())
	{
		if(index.column() == 1)
		{
			m_runButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
		}
		else if(index.column() == 2)
		{
			m_editButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
		}
	}
}

TestModelDelegate::~TestModelDelegate()
{
	delete m_runButton;
	delete m_editButton;
}
