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
#include "ScriptBacktraceModel.h"

#include <QDebug>
#include <QScriptContext>
#include <QScriptContextInfo>

ScriptBacktraceModel::ScriptBacktraceModel(QScriptContext* context, QObject* parent)
: QAbstractTableModel(parent)
{
	QScriptContext* currentContext = context;
	while(currentContext)
	{
		QScriptContextInfo info(currentContext);
		m_columns.append(info.columnNumber());
		m_fileNames.append(info.fileName());
		m_functions.append(info.functionName());
		m_lineNumbers.append(info.lineNumber());
		currentContext = currentContext->parentContext();
	}
}

int ScriptBacktraceModel::columnCount(const QModelIndex& index) const
{
	Q_UNUSED(index);
	return 4;
}

int ScriptBacktraceModel::rowCount(const QModelIndex& index) const
{
	if(index.isValid())
	{
		return 0;
	}
	return m_functions.count();
}

QVariant ScriptBacktraceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}
	switch(section)
	{
		case FunctionColumn:
			return tr("Function");
		case LineNumberColumn:
			return tr("Line");
		case FileNameColumn:
			return tr("File");
		case ColumnNumberColumn:
			return tr("Column");
	}
	return QVariant();
}

QVariant ScriptBacktraceModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}
	switch(role)
	{
		case Qt::DisplayRole:
			switch(index.column())
			{
				case FunctionColumn:
					return m_functions.at(index.row());
				case LineNumberColumn:
					return m_lineNumbers.at(index.row());
				case ColumnNumberColumn:
					return m_columns.at(index.row());
				case FileNameColumn:
					return m_fileNames.at(index.row());
			}
			break;
	}
	return QVariant();
}
