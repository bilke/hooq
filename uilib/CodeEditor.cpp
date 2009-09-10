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
#include "CodeEditor.h"
#include "CodeEditor_Margin.h"

#include <QDebug>
#include <QFont>
#include <QTextBlock>

CodeEditor::CodeEditor(QWidget* parent)
: QPlainTextEdit(parent)
, m_margin(new Margin(this))
{
	QFont codeFont(font());
	codeFont.setFamily("monospace");
	codeFont.setStyleHint(QFont::TypeWriter);
	setFont(codeFont);

	setViewportMargins(m_margin->sizeHint().width(), 0, 0, 0);
}

QSet<int> CodeEditor::breakpoints() const
{
	return m_breakpoints;
}

void CodeEditor::setBreakpoints(const QSet<int>& breakpoints)
{
	m_breakpoints = breakpoints;
	m_margin->update();
}

void CodeEditor::clearBreakpoints()
{
	setBreakpoints(QSet<int>());
}

void CodeEditor::clearLineHighlight()
{
	setExtraSelections(QList<QTextEdit::ExtraSelection>());
	viewport()->update();
}

void CodeEditor::highlightLine(int lineNumber)
{
	QList<QTextEdit::ExtraSelection> selections;

	QTextEdit::ExtraSelection selection;
	selection.format.setBackground(QColor(255, 140, 0)); // orange
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	selection.cursor = QTextCursor(document());

	const QTextBlock begin = document()->findBlockByNumber(lineNumber - 1);
	const QTextBlock end = document()->findBlockByNumber(lineNumber);

	selection.cursor.setPosition(begin.position());
	selection.cursor.setPosition(end.position(), QTextCursor::KeepAnchor);
	selections.append(selection);

	setExtraSelections(selections);
	viewport()->update();
}

void CodeEditor::toggleBreakpoint(int lineNumber)
{
	if(m_breakpoints.contains(lineNumber))
	{
		m_breakpoints.remove(lineNumber);
		emit breakpointRemoved(lineNumber);
	}
	else
	{
		m_breakpoints.insert(lineNumber);
		emit breakpointAdded(lineNumber);
	}
	m_margin->update();
}

void CodeEditor::resizeEvent(QResizeEvent* event)
{
	// Copied from Qt Code Editor example
	QPlainTextEdit::resizeEvent(event);
	const QRect contents = contentsRect();
	m_margin->setGeometry(
		QRect(
			QPoint(
				contents.left(),
				contents.top()
			),
			QSize(
				m_margin->sizeHint().width(),
				contents.height()
			)
		)
	);
}
