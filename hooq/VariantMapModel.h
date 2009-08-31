#pragma once

#include <QAbstractTableModel>
#include <QVariant>

class VariantMapModel : public QAbstractTableModel
{
	Q_OBJECT;
	public:
		enum DataRole
		{
			PropertyNameRole = Qt::UserRole,
			PropertyValueRole
		};
		VariantMapModel(const QVariantMap& map, QObject* parent);

		int columnCount(const QModelIndex& index = QModelIndex()) const;
		int rowCount(const QModelIndex& index = QModelIndex()) const;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	private:
		const QVariantMap m_map;
};
