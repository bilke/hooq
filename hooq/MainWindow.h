#pragma once

#include "ui_MainWindow.h"

#include <QMainWindow>

class Interpreter;
class TestModel;

class QModelIndex;
class QTemporaryFile;

namespace Hooq
{
	class Injector;
	class RemoteLogger;
}

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT;
	public:
		MainWindow(QWidget* parent = 0);
	private slots:
		void handleTestAction(const QModelIndex& index);
		void setTestSet(const QString&);
		void saveApplicationPath();
		void browseForApplication();
		void startRecording();
		void finishRecording();
		void updateAddState();
		void addTestSet();
	private:
		void editTestScript(const QModelIndex& index);
		void runTestScript(const QModelIndex& index);
		void populateTestSets();

		Hooq::Injector* m_hooqInjector;
		Hooq::RemoteLogger* m_hooqLogger;

		Interpreter* m_interpreter;
		TestModel* m_testModel;
		QTemporaryFile* m_xmlDump;
};
