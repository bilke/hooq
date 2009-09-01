#pragma once

#include "TestResult.h"

#include <QDialog>
#include <QList>

class QAbstractItemView;

class TestResultsWindow : public QDialog
{
	Q_OBJECT;
	public:
		TestResultsWindow(QWidget* parent);
		void setResults(const QList<TestResult>&);
	private:
		QAbstractItemView* m_view;
};
