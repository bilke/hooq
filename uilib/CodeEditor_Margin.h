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

#include "CodeEditor.h"

#include <QWidget>

class CodeEditor::Margin : public QWidget
{
	Q_OBJECT;
	public:
		Margin(CodeEditor* parent);
		QSize sizeHint() const;
	protected:
		virtual void mouseReleaseEvent(QMouseEvent* event);
		virtual void paintEvent(QPaintEvent* event);

		CodeEditor* m_editor;
};
