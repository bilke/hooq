#pragma once

#include "ui_TestSetDialog.h"

class QStringListModel;

class TestSetDialog : public QDialog, private Ui::TestSetDialog
{
	Q_OBJECT;
	public:
		TestSetDialog(QWidget* parent = 0);
		QString name() const;
		QString application() const;
		QStringList arguments() const;
		void setReadOnly(bool);
		bool isReadOnly() const;
	private slots:
		void browseForApplication();
	private:
		QStringListModel* m_model;
		bool m_readOnly;
};
