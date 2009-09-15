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
#include "MainWindow.h"

// Hooq
#include "ColumnClickMapper.h"
#include "Interpreter.h"
#include "Locations.h"
#include "ModelIndexKeyEventObserver.h"
#include "ScriptEditor.h"
#include "TestModel.h"
#include "TestResultsDialog.h"
#include "PushButtonDelegate.h"
#include "XmlToQtScript.h"

// hooqInjector
#include "RemoteConnection.h"
#include "RemoteLogger.h"
#include "PlatformInjector.h"

// Qt
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QStyle>
#include <QTemporaryFile>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, m_editor(0)
, m_hooqPlayInjector(new PlatformInjector(this))
, m_hooqRecordInjector(new PlatformInjector(this))
, m_hooqLogger(0)
, m_hooqPlayer(0)
, m_interpreter(new Interpreter(this))
, m_testModel(new TestModel(this))
, m_testRunning(false)
, m_testResultsWindow(new TestResultsDialog(this))
, m_xmlDump(0)
{
	m_editor = new ScriptEditor(m_interpreter->engine());
	setupUi(this);
	setStatusBar(0);

	populateTestSets();

	m_testList->setModel(m_testModel);
	PushButtonDelegate* delegate = new PushButtonDelegate(m_testList, this);
	delegate->addButton(1, QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
	delegate->addButton(2, QApplication::style()->standardIcon(QStyle::SP_FileIcon));
	m_testList->setItemDelegate(delegate);

	m_testList->header()->setResizeMode(0, QHeaderView::Stretch);
	m_testList->header()->setResizeMode(1, QHeaderView::Fixed);
	m_testList->header()->setResizeMode(2, QHeaderView::Fixed);
	m_testList->header()->setStretchLastSection(false);

	m_contextMenu = new QMenu(this);
	m_contextMenu->addAction(tr("Run"), this, SLOT(runCurrentTest()));
	m_contextMenu->addAction(tr("Edit"), this, SLOT(editCurrentTest()));
	m_contextMenu->addSeparator();
	m_contextMenu->addAction(tr("Delete"), this, SLOT(deleteCurrentTest()));

	setTestSet(m_testSetEdit->currentText());

	m_testList->setContextMenuPolicy(Qt::CustomContextMenu);

	QObject* deleteObserver = new ModelIndexKeyEventObserver(QKeySequence::Delete, m_testList);

	connect(
		deleteObserver,
		SIGNAL(released(QModelIndex)),
		SLOT(deleteCurrentTest())
	);

	connect(
		m_testList,
		SIGNAL(customContextMenuRequested(QPoint)),
		SLOT(showTestContextMenu(QPoint))
	);

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
		m_runAllButton,
		SIGNAL(clicked()),
		SLOT(runAllTests())
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
		m_hooqRecordInjector,
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

	connect(
		m_editor,
		SIGNAL(pickRequested()),
		m_interpreter,
		SLOT(pickObject())
	);

	connect(
		m_editor,
		SIGNAL(startRequested()),
		SLOT(runEditorTest())
	);

	connect(
		m_editor,
		SIGNAL(exceptionThrown(QString, QStringList)),
		SLOT(logException(QString, QStringList))
	);

	connect(
		m_interpreter,
		SIGNAL(objectPicked(ObjectInformation)),
		m_editor,
		SLOT(objectPicked(ObjectInformation))
	);

	connect(
		m_interpreter,
		SIGNAL(finished()),
		SLOT(testFinished())
	);

	connect(
		m_quit,
		SIGNAL(triggered()),
		qApp,
		SLOT(quit())
	);

	connect(
		m_aboutQt,
		SIGNAL(triggered()),
		qApp,
		SLOT(aboutQt())
	);

	connect(
		m_about,
		SIGNAL(triggered()),
		SLOT(about())
	);

	ColumnClickMapper* mapper = new ColumnClickMapper(m_testList);
	mapper->addMapping(1, this, SLOT(runTestScript(QModelIndex)));
	mapper->addMapping(2, this, SLOT(editTestScript(QModelIndex)));
}

void MainWindow::about()
{
	QMessageBox::about(
		this,
		tr("About Hooq"),
		tr(
			"<h1>About Hooq</h1>"
			"<p>Hooq is a cross-platform program for testing GUI applications written using Qt 4.</p>"
			"<p>Different parts of Hooq are licensed differently - see individual source files for information on which of the following license applies to the specific file.</p>"
			"<ul>"
			"<li>The GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.</li>"
			"<li>The GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.</li>"
			"</ul>"
			"<p>The following entitites have contributed code used by Hooq:</p>"
			"<ul>"
			"<li>Copyright &#169; 2009 Mendeley Limited &lt;copyright@mendeley.com&gt;</li>"
			"<li>Copyright &#169; 2009 Frederick Emmott &lt;mail@fredemmott.co.uk&gt;</li>"
			"<li>Copyright &#169; 2009 Nokia Corporation and/or its supsidary(-ies).</li>"
			"</ul>"
		)
	);
}

void MainWindow::runEditorTest()
{
	for(int i = 0; i < m_testModel->rowCount(); ++i)
	{
		const QModelIndex index = m_testModel->index(i, 0);
		if(index.data(TestModel::FilePathRole).toString() == m_editor->filePath())
		{
			runTestScript(index);
			break;
		}
	}
}

bool MainWindow::editTestScript(const QModelIndex& index)
{
	return editTestScript(index, EnsureVisible);
}

bool MainWindow::editTestScript(const QModelIndex& index, VisibleAction action)
{
	const bool cancelled = !m_editor->open(index.data(TestModel::FilePathRole).toString());
	if(cancelled)
	{
		return false;
	}

	m_editor->setWindowTitle(
		QString(
			"[*]%3 (%2) - %1"
		).arg(
			QCoreApplication::applicationName()
		).arg(
			m_testModel->testSet()
		).arg(
			index.data(TestModel::ScriptNameRole).toString()
		)
	);
	if(action == EnsureVisible)
	{
		m_editor->show();
	}
	return true;
}

void MainWindow::runTestScript(const QModelIndex& index)
{
	if(!editTestScript(index, IgnoreVisible))
	{
		// ScriptEditor had unsaved changes, prompted save/discard/cancel, user clicked cancel
		return;
	}
	delete m_hooqPlayer;
	m_testRunning = true;
	m_testList->setEnabled(false);

	const QString testName = index.data(TestModel::ScriptNameRole).toString();
	m_testResult = TestResult(testName);

	statusBar()->showMessage(tr("Running test '%1'...").arg(testName));

	m_interpreter->setScriptPath(index.data(TestModel::FilePathRole).toString());

	m_hooqPlayer = new Hooq::RemoteConnection(this);
	connect(
		m_hooqPlayer,
		SIGNAL(connected(QLocalSocket*)),
		m_interpreter,
		SLOT(run(QLocalSocket*))
	);
	m_hooqPlayer->start(QDir::fromNativeSeparators(m_applicationEdit->text()), m_hooqPlayInjector);
}

void MainWindow::logException(const QString& exception, const QStringList& backtrace)
{
	m_testResult = TestResult(m_testResult.name(), exception, backtrace);
}

void MainWindow::waitForEndOfTest()
{
	while(m_testRunning)
	{
		QApplication::processEvents();
	}
}

void MainWindow::testFinished()
{
	m_testRunning = false;
	m_testList->setEnabled(true);
	m_testResults.append(m_testResult);
}

void MainWindow::startRecording()
{
	delete m_hooqLogger;
	delete m_xmlDump;

	m_xmlDump = new QTemporaryFile();
	m_xmlDump->open();

	m_hooqLogger = new Hooq::RemoteLogger();
	m_hooqLogger->start(QDir::fromNativeSeparators(m_applicationEdit->text()), m_xmlDump, m_hooqRecordInjector);
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

	XmlToQtScript::Options options = XmlToQtScript::NoOptions;

	if(! m_recordMouseMovements->isChecked())
	{
		options |= XmlToQtScript::SkipMouseMovements;
	}
	if(m_simplifyStrings->isChecked())
	{
		options |= XmlToQtScript::SimplifyStrings;
	}
	if(m_objectVariables->isChecked())
	{
		options |= XmlToQtScript::ObjectVariables;
	}
	if(m_stringVariables->isChecked())
	{
		options |= XmlToQtScript::StringVariables;
	}

	XmlToQtScript parser;
	script.write(parser.parse(m_xmlDump, options).toUtf8());
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

void MainWindow::runCurrentTest()
{
	runTestScript(m_testList->currentIndex());
}

void MainWindow::runAllTests()
{
	m_testResults.clear();

	m_editor->setMode(ScriptEditor::Headless);
	m_testList->setEnabled(false);

	for(int i = 0; i < m_testModel->rowCount(); ++i)
	{
		QModelIndex index = m_testModel->index(i, 0);
		m_testList->setCurrentIndex(index);
		runCurrentTest();
		waitForEndOfTest();
	}

	m_testList->setEnabled(true);
	m_editor->setMode(ScriptEditor::Interactive);

	m_testResultsWindow->setResults(m_testResults);
	m_testResultsWindow->show();
}

void MainWindow::editCurrentTest()
{
	editTestScript(m_testList->currentIndex());
}

void MainWindow::deleteCurrentTest()
{
	const QModelIndex index = m_testList->currentIndex();
	if(!index.isValid())
	{
		return;
	}

	const QMessageBox::StandardButton result = QMessageBox::warning(
		this,
		tr("Delete Test?"),
		tr("Are you sure you want to delete the test '%1'?").arg(index.data().toString()),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No
	);

	if(result == QMessageBox::Yes)
	{
		QFile::remove(index.data(TestModel::FilePathRole).toString());
	}
}

void MainWindow::showTestContextMenu(const QPoint& position)
{
	const QModelIndex& index = m_testList->indexAt(position);
	if((!index.isValid()) || index.column() != 0)
	{
		return;
	}

	m_contextMenu->popup(m_testList->viewport()->mapToGlobal(position));
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
