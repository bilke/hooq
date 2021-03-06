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
#include "BacktraceWidget.h"

#include "ScriptBacktraceModel.h"

#include <QTableView>
#include <QVBoxLayout>

BacktraceWidget::BacktraceWidget(QWidget* parent)
: QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	QTableView* view = new QTableView(this);
	layout->addWidget(view);

	view->setHorizontalHeader(false);
	
	m_view = view;
}

void BacktraceWidget::setContext(QScriptContext* context)
{
	delete m_view->model();
	m_view->setModel(new ScriptBacktraceModel(context, this));
}
