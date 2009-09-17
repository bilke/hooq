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
#include "TestModel.h"

#include "Locations.h"

#include <QDebug>
#include <QDir>
#include <QSize>
#include <QFileSystemWatcher>

TestModel::TestModel(QObject* parent)
: QAbstractTableModel(parent)
, m_watcher(new QFileSystemWatcher(this))
{
	connect(
		m_watcher,
		SIGNAL(directoryChanged(QString)),
		SLOT(rescan())
	);
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

void TestModel::rescan()
{
	setTestSet(testSet());
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

	if(!m_watcher->directories().contains(testSetDir.path()))
	{
		m_watcher->addPath(testSetDir.path());
	}

	m_items = testSetDir.entryList(QStringList("*.qs"), QDir::Files, QDir::Name);
	m_items.replaceInStrings(".qs", QString());
	reset();
}

void TestModel::stopWatching(const QString& testSet)
{
	m_watcher->removePath(Locations::testSetLocation(testSet));
}

QVariant TestModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}

	Q_ASSERT(index.row() >= 0 && index.row() < m_items.count());
	Q_ASSERT(index.column() >= 0 && index.column() < columnCount());

	switch(role)
	{
		case FilePathRole:
			return Locations::scriptPath(testSet(), m_items.at(index.row()));
		case ScriptNameRole:
			return m_items.at(index.row());
	}

	switch(index.column())
	{
		case 0:
			if(role == Qt::DisplayRole)
			{
				return m_items.at(index.row());
			}
			else
			{
				return QVariant();
			}
		case 1:
			if(role == Qt::SizeHintRole)
			{
				return QSize(32, 32);
			}
			else if(role == Qt::ToolTipRole)
			{
				return tr("Run Test");
			}
		case 2:
			if(role == Qt::SizeHintRole)
			{
				return QSize(32, 32);
			}
			else if(role == Qt::ToolTipRole)
			{
				return tr("Edit Test");
			}
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
