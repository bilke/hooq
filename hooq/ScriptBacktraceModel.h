#pragma once

#include <QAbstractTableModel>
#include <QStringList>

class QScriptContext;

class ScriptBacktraceModel : public QAbstractTableModel
{
	Q_OBJECT;
	public:
		ScriptBacktraceModel(QScriptContext* context, QObject* parent);

		int columnCount(const QModelIndex& index = QModelIndex()) const;
		int rowCount(const QModelIndex& index = QModelIndex()) const;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	private:
		enum Columns
		{
			FunctionColumn,
			LineNumberColumn,
			ColumnNumberColumn,
			FileNameColumn
		};
		QStringList m_functions;
		QString m_fileNames;
		QList<int> m_columns;
		QList<int> m_lineNumbers;

};
