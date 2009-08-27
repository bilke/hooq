#include "MainWindow.h"

// Hooq
#include "Locations.h"
#include "TestModel.h"
#include "XmlToQtScript.h"

// hooqInjector
#include "RemoteLogger.h"
#include "PlatformInjector.h"

// Qt
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
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

	connect(
		m_testNameEdit,
		SIGNAL(textChanged(QString)),
		SLOT(updateAddState())
	);

	connect(
		m_addTestSetButton,
		SIGNAL(clicked()),
		SLOT(addTestSet())
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
	delete m_hooqLogger;
	m_hooqLogger = 0;
	
	m_xmlDump->close();
	m_xmlDump->open();

	QFile script(Locations::scriptPath(m_testModel->testSet(), m_testNameEdit->text()));
	script.open(QIODevice::WriteOnly | QFile::Truncate);
	Q_ASSERT(script.isOpen() && script.isWritable());

	XmlToQtScript parser;
	script.write(parser.parse(m_xmlDump).toUtf8());
	script.close();

	delete m_xmlDump;
	m_xmlDump = 0;

	m_testNameEdit->setText(QString());
}

void MainWindow::updateAddState()
{
	m_addTestButton->setEnabled(
		(!m_testModel->testSet().isEmpty())
		&&
		(!m_testNameEdit->text().isEmpty())
		&&
		(!m_applicationEdit->text().isEmpty())
		&&
		QFile::exists(QDir::fromNativeSeparators(m_applicationEdit->text()))
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

void MainWindow::addTestSet()
{
	const QString newName = QInputDialog::getText(this, tr("Test set name"), tr("Enter a name for your new test set"));
	if(!newName.isEmpty())
	{
		// todo: check unique
		m_testSetEdit->addItem(newName);
		m_testSetEdit->setCurrentIndex(m_testSetEdit->findText(newName));
		setTestSet(newName);
	}
}
