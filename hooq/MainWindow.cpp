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
#include "Interpreter.h"
#include "Locations.h"
#include "TestModel.h"
#include "TestModelDelegate.h"
#include "XmlToQtScript.h"

// hooqInjector
#include "RemotePlayback.h"
#include "RemoteLogger.h"
#include "PlatformInjector.h"

// Qt
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QString>
#include <QStringList>
#include <QTemporaryFile>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, m_hooqInjector(new PlatformInjector(this))
, m_hooqLogger(0)
, m_hooqPlayer(0)
, m_interpreter(new Interpreter(this))
, m_testModel(new TestModel(this))
, m_xmlDump(0)
{
	setupUi(this);
	setStatusBar(0);

	populateTestSets();

	m_testList->setModel(m_testModel);
	TestModelDelegate* delegate = new TestModelDelegate(m_testList, this);
	m_testList->setItemDelegate(delegate);

	m_testList->setMouseTracking(true);

	m_testList->header()->setResizeMode(0, QHeaderView::Stretch);
	m_testList->header()->setResizeMode(1, QHeaderView::Fixed);
	m_testList->header()->setResizeMode(2, QHeaderView::Fixed);
	m_testList->header()->setStretchLastSection(false);

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

	connect(
		m_testList,
		SIGNAL(clicked(QModelIndex)),
		SLOT(handleTestAction(QModelIndex))
	);
}

void MainWindow::handleTestAction(const QModelIndex& index)
{
	if(!index.isValid())
	{
		return;
	}
	switch(index.column())
	{
		case 0:
			return;
		case 1:
			runTestScript(index);
			return;
		case 2:
			editTestScript(index);
			return;
	}
}

void MainWindow::editTestScript(const QModelIndex& index)
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(index.data(TestModel::FilePathRole).toString()));
}

void MainWindow::runTestScript(const QModelIndex& index)
{
	delete m_hooqPlayer;
	delete m_xmlDump;

	m_xmlDump = new QTemporaryFile();
	m_xmlDump->open();

	m_interpreter->run(index.data(TestModel::FilePathRole).toString(), m_xmlDump);

	m_xmlDump->close();
	m_xmlDump->open();

	m_hooqPlayer = new Hooq::RemotePlayback();
	m_hooqPlayer->start(QDir::fromNativeSeparators(m_applicationEdit->text()), m_xmlDump, m_hooqInjector);
	
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
