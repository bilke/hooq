#include "MainWindow.h"

// Hooq
#include "Locations.h"
#include "TestModel.h"

// hooqInjector
#include "RemoteLogger.h"
#include "PlatformInjector.h"

// Qt
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, m_hooqInjector(new PlatformInjector(this))
, m_hooqLogger(0)
, m_testModel(new TestModel(this))
, m_xmlDump(0)
{
	setupUi(this);
	setStatusBar(0);

	populateTestSets();


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

	connect(
		m_addTestButton,
		SIGNAL(clicked()),
		SLOT(startRecording())
	);

	connect(
		m_hooqInjector,
		SIGNAL(finished(int)),
		SLOT(finishRecording())
	);
}

void MainWindow::startRecording()
{
	delete m_hooqLogger;
	delete m_xmlDump;

	m_xmlDump = new QTemporaryFile();
	m_xmlDump->open();

	m_hooqLogger = new Hooq::RemoteLogger();
	m_hooqLogger->start(QDir::fromNativeSeparators(m_applicationEdit->text()), m_xmlDump, m_hooqInjector);
}

void MainWindow::finishRecording()
{
	qDebug() << "Saved XML to" << m_xmlDump->fileName();
	m_xmlDump->close();
	delete m_hooqLogger;
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
