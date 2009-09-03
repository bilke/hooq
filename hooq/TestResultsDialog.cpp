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
#include "TestResultsDialog.h"

#include "ColumnClickMapper.h"
#include "PushButtonDelegate.h"
#include "TestFailureDialog.h"
#include "TestResultsModel.h"

#include <QDialogButtonBox>
#include <QHeaderView>
#include <QProgressBar>
#include <QTreeView>
#include <QVBoxLayout>

TestResultsDialog::TestResultsDialog(QWidget* parent)
: QDialog(parent)
, m_informationDialog(new TestFailureDialog(this))
{
	m_informationDialog->setModal(true);

	setWindowTitle(tr("Test Results"));
	QVBoxLayout* layout = new QVBoxLayout(this);

	m_progress = new QProgressBar(this);
	layout->addWidget(m_progress);
	m_progress->setFormat("%v of %m passed (%p%)");

	QTreeView* view = new QTreeView(this);
	m_view = view;
	layout->addWidget(view);

	PushButtonDelegate* delegate = new PushButtonDelegate(m_view, this);
	delegate->addButton(TestResultsModel::columnNumber(TestResultsModel::InformationColumn), tr("Information"));
	delegate->setEnabledRole(TestResultsModel::ButtonEnabledRole);
	view->setItemDelegate(delegate);

	view->header()->setStretchLastSection(false);
	view->setExpandsOnDoubleClick(false);
	view->setItemsExpandable(false);
	view->setRootIsDecorated(false);
	view->setUniformRowHeights(true);

	ColumnClickMapper* mapper = new ColumnClickMapper(m_view);
	mapper->addMapping(TestResultsModel::columnNumber(TestResultsModel::InformationColumn), this, SLOT(showInformation(QModelIndex)), TestResultsModel::ButtonEnabledRole);

	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this);
	buttons->setCenterButtons(true);
	layout->addWidget(buttons);
	connect(
		buttons,
		SIGNAL(accepted()),
		SLOT(accept())
	);
}

void TestResultsDialog::showInformation(const QModelIndex& index)
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

void TestResultsDialog::setResults(const QList<TestResult>& results)
{
	m_progress->setMaximum(results.count());
	int passing = 0;
	Q_FOREACH(const TestResult& result, results)
	{
		if(result.passed())
		{
			++passing;
		}
	}
	m_progress->setValue(passing);

	QAbstractItemModel* oldModel = m_view->model();
	m_view->setModel(new TestResultsModel(results, this));
	m_view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::TestNameColumn), QHeaderView::Stretch);
	m_view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::TestResultColumn), QHeaderView::Fixed);
	m_view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::InformationColumn), QHeaderView::Fixed);
	delete oldModel;
}
