#include "VariantMapModel.h"

#include <QApplication>
#include <QFontMetrics>
#include <QSize>
#include <QStringList>

VariantMapModel::VariantMapModel(const QVariantMap& map, QObject* parent)
: QAbstractTableModel(parent)
, m_map(map)
{
}

int VariantMapModel::columnCount(const QModelIndex& index) const
{
	Q_UNUSED(index);
	return 4;
}

int VariantMapModel::rowCount(const QModelIndex& index) const
{
	if(index.isValid())
	{
		return 0;
	}
	return m_map.count();
}

QVariant VariantMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}
	switch(section)
	{
		case 0:
			return tr("Name");
		case 1:
			return tr("Value");
		case 2:
			return tr("Fetch");
		case 3:
			return tr("Compare");
	}
	return QVariant();
}

QVariant VariantMapModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}
	const QString key = m_map.keys().at(index.row());
	switch(role)
	{
		case Qt::DisplayRole:
			switch(index.column())
			{
				case 0:
					return key;
				case 1:
					return m_map.value(key).toString();
				default:
					return QVariant();
			}
			break;
		case Qt::SizeHintRole:
		{
			QFontMetrics metrics(QApplication::fontMetrics());
			switch(index.column())
			{
				case 0:
					return metrics.size(0, key);
				case 1:
					return metrics.size(0, m_map.value(key).toString());
				default:
					return QSize(32, 32);
			}
		}
		case PropertyNameRole:
			return key;
		case PropertyValueRole:
			return m_map.value(key);
	}
	return QVariant();
}
