#pragma once

#include "ui_MainWindow.h"

#include <QMainWindow>

class TestModel;
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
		void setTestSet(const QString&);
		void saveApplicationPath();
		void browseForApplication();
		void startRecording();
		void finishRecording();
		void updateAddState();
		void addTestSet();
	private:
		void populateTestSets();

		Hooq::Injector* m_hooqInjector;
		Hooq::RemoteLogger* m_hooqLogger;

		TestModel* m_testModel;
		QTemporaryFile* m_xmlDump;
};
