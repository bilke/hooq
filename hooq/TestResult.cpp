#include "TestResult.h"

TestResult::TestResult(const QString& name)
: m_name(name)
, m_passed(true)
{
}

TestResult::TestResult(const QString& name, const QString& error, const QStringList& backtrace)
: m_backtrace(backtrace)
, m_error(error)
, m_name(name)
, m_passed(false)
{
}

QString TestResult::name() const
{
	return m_name;
}

QString TestResult::error() const
{
	return m_error;
}

QStringList TestResult::backtrace() const
{
	return m_backtrace;
}
