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
#include "TestResultsModel.h"

#include <QApplication>
#include <QFontMetrics>
#include <QSize>
#include <QStringList>

TestResultsModel::TestResultsModel(const QList<TestResult>& results, QObject* parent)
: QAbstractTableModel(parent)
, m_testResults(results)
{
}

int TestResultsModel::columnCount(const QModelIndex& index) const
{
	Q_UNUSED(index);
	return 3;
}

int TestResultsModel::columnNumber(ColumnRole role)
{
	return static_cast<int>(role);
}

int TestResultsModel::rowCount(const QModelIndex& index) const
{
	if(index.isValid())
	{
		return 0;
	}
	return m_testResults.count();
}

QVariant TestResultsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}
	switch(section)
	{
		case TestNameColumn:
			return tr("Test");
		case TestResultColumn:
			return tr("Result");
		case InformationColumn:
			return tr("Information");
	}
	return QVariant();
}

QVariant TestResultsModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}
	Q_ASSERT(index.row() < m_testResults.count() && index.row() >= 0);
	const TestResult result = m_testResults.at(index.row());
	switch(role)
	{
		case Qt::BackgroundRole:
			if(result.passed())
			{
				return Qt::green;
			}
			else
			{
				return Qt::red;
			}
		case Qt::FontRole:
			if(!result.passed())
			{
				QFont font(QApplication::font());
				font.setBold(true);
				return font;
			}
			return QVariant();
		case Qt::DisplayRole:
			switch(index.column())
			{
				case TestNameColumn:
					return result.name();
				case TestResultColumn:
					if(result.passed())
					{
						return tr("Passed");
					}
					else
					{
						return tr("FAILED");
					}
				default:
					return QVariant();
			}
			break;
		case Qt::SizeHintRole:
		{
			QFontMetrics metrics(QApplication::fontMetrics());
			switch(index.column())
			{
				case TestNameColumn:
					return metrics.size(0, result.name());
				case TestResultColumn:
					return metrics.size(0, data(index).toString());
				default:
					return QSize(32, 32);
			}
		}
		case ButtonEnabledRole:
			return !result.passed();
		case BacktraceRole:
			return result.backtrace();
		case ErrorRole:
			return result.error();
		case TestNameRole:
			return result.name();
	}
	return QVariant();
}
