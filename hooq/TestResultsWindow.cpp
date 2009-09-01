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
#include "TestResultsWindow.h"

#include "PushButtonDelegate.h"
#include "TestFailureDialog.h"
#include "TestResultsModel.h"

#include <QHeaderView>
#include <QTreeView>
#include <QVBoxLayout>

TestResultsWindow::TestResultsWindow(QWidget* parent)
: QDialog(parent)
, m_informationDialog(new TestFailureDialog(this))
{
	m_informationDialog->setModal(true);

	setWindowTitle(tr("Test Results"));
	QVBoxLayout* layout = new QVBoxLayout(this);
	QTreeView* view = new QTreeView(this);
	m_view = view;
	layout->addWidget(view);

	PushButtonDelegate* delegate = new PushButtonDelegate(m_view, this);
	delegate->addButton(TestResultsModel::columnNumber(TestResultsModel::InformationColumn), tr("Information"));
	delegate->setEnabledRole(TestResultsModel::ButtonEnabledRole);
	view->setItemDelegate(delegate);

	view->header()->setStretchLastSection(false);
	view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::TestNameColumn), QHeaderView::Stretch);
	view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::TestResultColumn), QHeaderView::Fixed);
	view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::InformationColumn), QHeaderView::Fixed);
	view->setRootIsDecorated(false);

	connect(
		view,
		SIGNAL(clicked(QModelIndex)),
		SLOT(showInformation(QModelIndex))
	);
}

void TestResultsWindow::showInformation(const QModelIndex& index)
{
	if(!index.isValid())
	{
		return;
	}
	if(index.column() != TestResultsModel::columnNumber(TestResultsModel::InformationColumn))
	{
		return;
	}
	const QString name = index.data(TestResultsModel::TestNameRole).toString();
	const QString error = index.data(TestResultsModel::ErrorRole).toString();
	const QStringList backtrace = index.data(TestResultsModel::BacktraceRole).toStringList();
	m_informationDialog->show(name, error, backtrace);
}

void TestResultsWindow::setResults(const QList<TestResult>& results)
{
	QAbstractItemModel* oldModel = m_view->model();
	m_view->setModel(new TestResultsModel(results, this));
	delete oldModel;
}
