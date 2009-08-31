#include "ScriptBacktraceModel.h"

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
	return 3;
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
