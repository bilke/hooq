#include "MainWindow.h"

#include "Locations.h"
#include "TestModel.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
{
	setupUi(this);
	setStatusBar(0);

	populateTestSets();

	m_testModel = new TestModel(this);

	m_testList->setModel(m_testModel);

	setTestSet(m_testSetEdit->currentText());

	connect(
		m_testSetEdit,
		SIGNAL(activated(QString)),
		SLOT(setTestSet(QString))
	);

	connect(
		m_applicationEdit,
		SIGNAL(editingFinished()),
		SLOT(saveApplicationPath())
	);

	connect(
		m_applicationBrowseButton,
		SIGNAL(clicked()),
		SLOT(browseForApplication())
	);
}

void MainWindow::browseForApplication()
{
	const QString applicationPath = QFileDialog::getOpenFileName(this, tr("Application for %1").arg(m_testModel->testSet()));
	if(!applicationPath.isEmpty())
	{
		const QString native = QDir::toNativeSeparators(applicationPath);
		m_applicationEdit->setText(native);
		Locations::setApplicationPath(m_testModel->testSet(), applicationPath);
	}
}

void MainWindow::saveApplicationPath()
{
	Locations::setApplicationPath(m_testModel->testSet(), QDir::fromNativeSeparators(m_applicationEdit->text()));
}

void MainWindow::setTestSet(const QString& testSet)
{
	m_testModel->setTestSet(testSet);
	m_applicationEdit->setText(QDir::toNativeSeparators(Locations::applicationPath(testSet)));
}

void MainWindow::populateTestSets()
{
	QDir dataDir(Locations::testSetsLocation());
	Q_FOREACH(const QString& set, dataDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		m_testSetEdit->addItem(set);
	}
}
