#pragma once

#include <QString>

class TestSetBackup
{
	public:
		static bool backup(const QString& testSet, const QString& file);
		static QString restore(const QString& file);
	private:
		static const char m_magic[];
};
