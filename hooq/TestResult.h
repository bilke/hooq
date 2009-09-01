#pragma once

#include <QString>
#include <QStringList>

class TestResult
{
	public:
		TestResult(const QString& name);
		TestResult(const QString& name, const QString& error, const QStringList& backtrace);
		QString name() const;
		bool passed() const;
		QString error() const;
		QStringList backtrace() const;
	private:
		QStringList m_backtrace;
		QString m_error;
		QString m_name;
		bool m_passed;
};
