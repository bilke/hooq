/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
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
