/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Nokia Corporation and/or its subsidiary(-ies).
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
#include "CodeEditor_Margin.h"

#include <QDebug>
#include <QFontMetrics>
#include <QLinearGradient>
#include <QPainter>
#include <QTextBlock>

CodeEditor::Margin::Margin(CodeEditor* parent)
: QWidget(parent)
, m_editor(parent)
{
	connect(
		m_editor,
		SIGNAL(updateRequest(QRect, int)),
		SLOT(repaint())
	);
}

void CodeEditor::Margin::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mouseReleaseEvent(event);
	// Find out where we were clicked, in terms of line numbers
	//
	QTextBlock block = m_editor->firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = static_cast<int>(m_editor->blockBoundingGeometry(block).translated(m_editor->contentOffset()).top());
	int bottom = top + static_cast<int>(m_editor->blockBoundingRect(block).height());

	while(block.isValid())
	{
		if(block.isVisible() && bottom > event->y())
		{
			m_editor->toggleBreakpoint(blockNumber + 1);
			return;
		}

		block = block.next();
		top = bottom;
		bottom = top + static_cast<int>(m_editor->blockBoundingRect(block).height());
		++blockNumber;
	}
}

QSize CodeEditor::Margin::sizeHint() const
{
	const QFontMetrics metrics(font());
	return QSize(
		metrics.width("99999"),
		0
	);
}

void CodeEditor::Margin::paintEvent(QPaintEvent* event)
{
	const QSet<int> breakpoints = m_editor->breakpoints();

	// Copied from Qt Code Editor example
	QPainter painter(this);
	painter.fillRect(event->rect(), Qt::lightGray);
	painter.setRenderHint(QPainter::Antialiasing);
	QTextBlock block = m_editor->firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = static_cast<int>(m_editor->blockBoundingGeometry(block).translated(m_editor->contentOffset()).top());
	int bottom = top + static_cast<int>(m_editor->blockBoundingRect(block).height());

	const int fontHeight = fontMetrics().height();

	while(block.isValid() && top <= event->rect().bottom())
	{
		if(block.isVisible() && bottom >= event->rect().top())
		{
			const int lineNumber = blockNumber + 1;
			const QString number = QString::number(lineNumber);
			painter.setPen(Qt::black);
			painter.drawText(
				0,
				top,
				width() - 1, // padding
				fontHeight,
				Qt::AlignRight,
				number
			);
			
			if(breakpoints.contains(lineNumber))
			{
				painter.save();
				const QRectF area = QRect(QPoint(0, top), QSize(fontHeight, fontHeight));
				QColor red(Qt::red);
				QLinearGradient brush;
				brush.setColorAt(0, red.darker());
				brush.setColorAt(1, red);
				brush.setStart(area.topLeft());
				brush.setFinalStop(area.bottomRight());
				painter.setPen(Qt::black);
				painter.setBrush(brush);
				painter.drawEllipse(area.translated(1.5, 0.5));
				painter.restore();
			}
		}

		block = block.next();
		top = bottom;
		bottom = top + static_cast<int>(m_editor->blockBoundingRect(block).height());
		++blockNumber;
	}
}
