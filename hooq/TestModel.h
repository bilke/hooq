#pragma once

#include <QAbstractTableModel>
#include <QStringList>

class TestModel : public QAbstractTableModel
{
	Q_OBJECT;
	public:
		TestModel(QObject* parent);

		QString testSet() const;

		int columnCount(const QModelIndex& index = QModelIndex()) const;
		int rowCount(const QModelIndex& index = QModelIndex()) const;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	public slots:
		void setTestSet(const QString&);
	private:
		QStringList m_items;
		QString m_testSet;
};
