#include "TestModel.h"

#include "Locations.h"

#include <QDir>

TestModel::TestModel(QObject* parent)
: QAbstractTableModel(parent)
{
}

int TestModel::columnCount(const QModelIndex& index) const
{
	Q_UNUSED(index);
	return 3;
}

int TestModel::rowCount(const QModelIndex& index) const
{
	if(index.isValid())
	{
		return 0;
	}
	return m_items.count();
}

QString TestModel::testSet() const
{
	return m_testSet;
}

void TestModel::setTestSet(const QString& testSet)
{
	m_testSet = testSet;

	if(testSet.isEmpty())
	{
		return;
	}

	QDir testSetDir(Locations::testSetLocation(testSet));
	if(!testSetDir.exists())
	{
		testSetDir.mkpath("."); // relative to the dir
	}

	m_items = testSetDir.entryList(QDir::Files);
	m_items.replaceInStrings(".qs", QString());
	m_items.sort();
}

QVariant TestModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}
	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}

	Q_ASSERT(index.row() >= 0 && index.row() < m_items.count());
	Q_ASSERT(index.column() >= 0 && index.column() < columnCount());
	switch(index.column())
	{
		case 0:
			return m_items.at(index.row());
		default:
			break;
	}

	return QVariant();
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
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
			return tr("Run");
		case 2:
			return tr("Edit");
		default:
			return QVariant();
	}
}
