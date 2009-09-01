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
#include "TestFailureDialog.h"

#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>

TestFailureDialog::TestFailureDialog(QWidget* parent)
: QDialog(parent)
{
	QVBoxLayout* layout = new QVBoxLayout(this);

	m_nameLabel = new QLabel(this);
	layout->addWidget(m_nameLabel);

	m_errorLabel = new QLabel(this);
	m_errorLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	m_errorLabel->setFrameShape(QFrame::StyledPanel);
	m_errorLabel->setFrameShadow(QFrame::Sunken);
	m_errorLabel->setFont(QFont("monospace"));
	layout->addWidget(m_errorLabel);

	m_backtraceModel = new QStringListModel(this);
	QListView* view = new QListView(this);
	layout->addWidget(view);
	view->setModel(m_backtraceModel);
}

void TestFailureDialog::show(const QString& name, const QString& error, const QStringList& backtrace)
{
	m_nameLabel->setText(tr("Test '%1' failed:").arg(name));
	m_errorLabel->setText(error);
	m_backtraceModel->setStringList(backtrace);
	QDialog::show();
}
