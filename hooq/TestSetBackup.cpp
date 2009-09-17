#include "TestSetBackup.h"

#include "Locations.h"

#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QStringList>

const char TestSetBackup::m_magic[] = {'%', 'H', 'o', 'o', 'q', 0x00, 0x55, 0xaa, 0xff};

bool TestSetBackup::backup(const QString& testSet, const QString& path)
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
		stream.setVersion(QDataStream::Qt_4_5);

		stream << testSet;
		const QString directoryPath = Locations::testSetLocation(testSet);
		Q_FOREACH(const QString& file, QDir(directoryPath).entryList(QDir::Files | QDir::NoDotAndDotDot))
		{
			QFile testFile(directoryPath + "/" + file);
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

QString TestSetBackup::restore(const QString& path)
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
	stream.setVersion(QDataStream::Qt_4_5);
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

	const QString directoryPath = Locations::testSetLocation(testSet) + "/";
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
