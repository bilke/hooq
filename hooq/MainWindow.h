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
#pragma once

#include "ui_MainWindow.h"

#include "TestResult.h"

#include <QList>
#include <QMainWindow>

class Interpreter;
class ScriptEditor;
class TestModel;
class TestResultsDialog;

class QMenu;
class QModelIndex;
class QTemporaryFile;

namespace Hooq
{
	class Injector;
	class RemoteConnection;
	class RemoteLogger;
}

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT;
	public:
		MainWindow(QWidget* parent = 0);
	private slots:
		void logException(const QString& exception, const QStringList& backtrace);
		void runCurrentTest();
		void editCurrentTest();
		void deleteCurrentTest();

		void handleTestAction(const QModelIndex& index);
		void setTestSet(const QString&);
		void saveApplicationPath();
		void browseForApplication();
		void startRecording();
		void finishRecording();
		void updateAddState();
		void addTestSet();
		void showTestContextMenu(const QPoint& position);
		void runAllTests();
		void testFinished();
	private:
		void waitForEndOfTest();
		void editTestScript(const QModelIndex& index);
		void runTestScript(const QModelIndex& index);
		void populateTestSets();

		QMenu* m_contextMenu;

		ScriptEditor* m_editor;

		Hooq::Injector* m_hooqPlayInjector;
		Hooq::Injector* m_hooqRecordInjector;
		Hooq::RemoteLogger* m_hooqLogger;
		Hooq::RemoteConnection* m_hooqPlayer;

		Interpreter* m_interpreter;
		TestModel* m_testModel;
		bool m_testRunning;

		TestResult m_testResult;
		QList<TestResult> m_testResults;
		TestResultsDialog* m_testResultsWindow;

		QTemporaryFile* m_xmlDump;
};
