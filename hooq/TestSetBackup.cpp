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
#include "TestSetBackup.h"

#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QStringList>

const char TestSetBackup::m_magic[] = {'%', 'H', 'o', 'o', 'q', 0x00, 0x55, 0xaa, 0xff};

bool TestSetBackup::backup(const QString& testSet, const QString& path, const QString& rootDirectory)
{
	QFile file(path);
	const bool fileOpen = file.open(QIODevice::WriteOnly | QFile::Truncate);
	Q_ASSERT(fileOpen);
	if(!fileOpen)
	{
		return false;
	}
	file.write(m_magic, sizeof(m_magic));

	QByteArray uncompressed;

	{
		QDataStream stream(&uncompressed, QIODevice::WriteOnly);
		stream.setVersion(QDataStream::Qt_4_4);

		stream << testSet;
		const QString directory(rootDirectory + "/" + testSet + "/");
		Q_FOREACH(const QString& file, QDir(directory).entryList(QDir::Files | QDir::NoDotAndDotDot))
		{
			QFile testFile(directory + file);
			const bool success = testFile.open(QIODevice::ReadOnly);
			Q_ASSERT(success);
			if(!success)
			{
				return false;
			}
			stream << file;
			stream << testFile.readAll();
		}
	}

	file.write(qCompress(uncompressed, 9));

	return true;
}

QString TestSetBackup::identify(const QString& path)
{
	QFile file(path);
	const bool fileOpen = file.open(QIODevice::ReadOnly);
	Q_ASSERT(fileOpen);
	if(file.size() < static_cast<qint64>(sizeof(m_magic)) || !fileOpen)
	{
		return QString();
	}
	if(file.read(sizeof(m_magic)) != QByteArray(m_magic, sizeof(m_magic)))
	{
		qDebug() << Q_FUNC_INFO << "Bad magic";
		return QString();
	}

	const QByteArray uncompressed = qUncompress(file.readAll());

	QDataStream stream(uncompressed);
	stream.setVersion(QDataStream::Qt_4_4);
	QString testSet;
	stream >> testSet;

	return testSet;
}

QStringList TestSetBackup::list(const QString& path)
{
	QFile file(path);
	const bool fileOpen = file.open(QIODevice::ReadOnly);
	Q_ASSERT(fileOpen);
	if(file.size() < static_cast<qint64>(sizeof(m_magic)) || !fileOpen)
	{
		return QStringList();
	}
	if(file.read(sizeof(m_magic)) != QByteArray(m_magic, sizeof(m_magic)))
	{
		qDebug() << Q_FUNC_INFO << "Bad magic";
		return QStringList();
	}

	QStringList tests;

	const QByteArray uncompressed = qUncompress(file.readAll());

	QDataStream stream(uncompressed);
	stream.setVersion(QDataStream::Qt_4_4);
	QString testSet;
	stream >> testSet;

	Q_ASSERT(!testSet.isEmpty());
	if(testSet.isEmpty())
	{
		return QStringList();
	}

	while(!stream.atEnd())
	{
		QString testName;
		stream >> testName;
		Q_ASSERT(testName.endsWith(".qs"));
		Q_ASSERT(!stream.atEnd());
		if(stream.atEnd() || !testName.endsWith(".qs"))
		{
			return QStringList();
		}
		QByteArray data;
		stream >> data;
		Q_UNUSED(data);
		tests.append(testName);
	}

	return tests;
}

QString TestSetBackup::restore(const QString& path, const QString& rootDirectory)
{
	QFile file(path);
	const bool fileOpen = file.open(QIODevice::ReadOnly);
	Q_ASSERT(fileOpen);
	if(file.size() < static_cast<qint64>(sizeof(m_magic)) || !fileOpen)
	{
		return QString();
	}
	if(file.read(sizeof(m_magic)) != QByteArray(m_magic, sizeof(m_magic)))
	{
		qDebug() << Q_FUNC_INFO << "Bad magic";
		return QString();
	}

	QMap<QString, QByteArray> tests; // buffer to make atomic

	const QByteArray uncompressed = qUncompress(file.readAll());

	QDataStream stream(uncompressed);
	stream.setVersion(QDataStream::Qt_4_4);
	QString testSet;
	stream >> testSet;

	Q_ASSERT(!testSet.isEmpty());
	if(testSet.isEmpty())
	{
		return QString();
	}

	while(!stream.atEnd())
	{
		QString testName;
		stream >> testName;
		Q_ASSERT(testName.endsWith(".qs"));
		Q_ASSERT(!stream.atEnd());
		if(stream.atEnd() || !testName.endsWith(".qs"))
		{
			return QString();
		}
		QByteArray data;
		stream >> data;
		tests.insert(testName, data);
	}

	const QString directoryPath = rootDirectory + "/" + testSet + "/";
	QDir().mkpath(directoryPath);

	for(QMap<QString, QByteArray>::ConstIterator it = tests.constBegin(); it != tests.constEnd(); ++it)
	{
		QFile testFile(directoryPath + it.key());
		const bool openedTestFile = testFile.open(QIODevice::WriteOnly | QFile::Truncate);
		Q_ASSERT(openedTestFile);
		if(!openedTestFile)
		{
			return QString();
		}
		testFile.write(it.value());
	}

	return testSet;
}
