#pragma once

#include "ui_MainWindow.h"

#include <QMainWindow>

class TestModel;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT;
	public:
		MainWindow(QWidget* parent = 0);
	private slots:
		void setTestSet(const QString&);
		void saveApplicationPath();
		void browseForApplication();
	private:
		void populateTestSets();

		TestModel* m_testModel;
};
