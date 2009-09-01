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
#include "TestResult.h"

TestResult::TestResult(const QString& name)
: m_name(name)
, m_passed(true)
{
}

TestResult::TestResult()
: m_passed(false)
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

bool TestResult::passed() const
{
	return m_passed;
}
