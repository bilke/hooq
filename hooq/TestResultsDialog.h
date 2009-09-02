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

#include "TestResult.h"

#include <QDialog>
#include <QList>

class TestFailureDialog;

class QTreeView;
class QModelIndex;
class QProgressBar;

class TestResultsDialog : public QDialog
{
	Q_OBJECT;
	public:
		TestResultsDialog(QWidget* parent);
		void setResults(const QList<TestResult>&);
	private slots:
		void showInformation(const QModelIndex& index);
	private:
		TestFailureDialog* m_informationDialog;
		QTreeView* m_view;
		QProgressBar* m_progress;
};
