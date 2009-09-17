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
		void about();
		void logException(const QString& exception, const QStringList& backtrace);

		void updateActionStates();
		void showTestContextMenu(const QPoint& position);

		void setTestSet(const QString&);

		void addTestSet();
		void editTestSet();
		void removeTestSet();

		void runAllTests();
		void runEditorTest();
		void runCurrentTest();
		void runTestScript(const QModelIndex& index);
		void testFinished();

		void editCurrentTest();
		bool editTestScript(const QModelIndex& index);

		void startRecording();
		void finishRecording();

		void deleteCurrentTest();

		void exportCurrentSet();
		void importTestSet();
	private:
		enum VisibleAction
		{
			EnsureVisible,
			IgnoreVisible
		};

		void saveApplicationArguments(const QString& testSet, const QStringList& arguments);
		QStringList applicationArguments(const QString& testSet);

		QStringList m_arguments;

		QString applicationPath() const;
		void setApplicationPath(const QString&);
		bool editTestScript(const QModelIndex& index, VisibleAction);
		void waitForEndOfTest();
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

		QString m_applicationPath;
};
