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
#pragma once

#include <QPlainTextEdit>
#include <QSet>

class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT;
	public:
		CodeEditor(QWidget* parent);
		void resizeEvent(QResizeEvent* event);

		QSet<int> breakpoints() const;
		void setBreakpoints(const QSet<int>&);
		void clearBreakpoints();

		void highlightLine(int lineNumber);
		void clearLineHighlight();
	public slots:
		void toggleBreakpoint(int lineNumber);
	signals:
		void breakpointAdded(int lineNumber);
		void breakpointRemoved(int lineNumber);
	private:
		class Margin;
		Margin* m_margin;

		QSet<int> m_breakpoints;
};
