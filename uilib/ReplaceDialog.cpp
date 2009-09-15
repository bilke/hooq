#include "ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget* parent)
: QDialog(parent)
{
	setupUi(this);
}

QTextDocument::FindFlags ReplaceDialog::flags() const
{
	QTextDocument::FindFlags flags;
	if(!m_caseInsensitive->isChecked())
	{
		flags |= QTextDocument::FindCaseSensitively;
	}
	if(m_wholeWords->isChecked())
	{
		flags |= QTextDocument::FindWholeWords;
	}
	return flags;
}

QString ReplaceDialog::searchText() const
{
	return m_find->text();
}

QString ReplaceDialog::replacementText() const
{
	return m_replace->text();
}
