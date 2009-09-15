#pragma once

#include "ui_ReplaceDialog.h"

#include <QTextDocument>

class ReplaceDialog : public QDialog, private Ui::ReplaceDialog
{
	Q_OBJECT;
	public:
		ReplaceDialog(QWidget* parent = 0);
		QTextDocument::FindFlags flags() const;
		QString searchText() const;
		QString replacementText() const;
};
