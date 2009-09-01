#include "TestResultsWindow.h"

#include "PushButtonDelegate.h"
#include "TestResultsModel.h"

#include <QHeaderView>
#include <QTreeView>
#include <QVBoxLayout>

TestResultsWindow::TestResultsWindow(QWidget* parent)
: QDialog(parent)
{
	setWindowTitle(tr("Test Results"));
	QVBoxLayout* layout = new QVBoxLayout(this);
	QTreeView* view = new QTreeView(this);
	m_view = view;
	layout->addWidget(view);

	PushButtonDelegate* delegate = new PushButtonDelegate(m_view, this);
	delegate->addButton(TestResultsModel::columnNumber(TestResultsModel::BacktraceColumn), tr("Backtrace"));
	delegate->setEnabledRole(TestResultsModel::ButtonEnabledRole);
	view->setItemDelegate(delegate);

	view->header()->setStretchLastSection(false);
	view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::TestNameColumn), QHeaderView::Stretch);
	view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::TestResultColumn), QHeaderView::Fixed);
	view->header()->setResizeMode(TestResultsModel::columnNumber(TestResultsModel::BacktraceColumn), QHeaderView::Fixed);
	view->setRootIsDecorated(false);
}

void TestResultsWindow::setResults(const QList<TestResult>& results)
{
	QAbstractItemModel* oldModel = m_view->model();
	m_view->setModel(new TestResultsModel(results, this));
	delete oldModel;
}
