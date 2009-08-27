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
	return 0; // FIXME
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

	// TODO: update
}

QVariant TestModel::data(const QModelIndex& index, int role) const
{
	// FIXME
	Q_UNUSED(index);
	Q_UNUSED(role);
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
