#pragma once

#include <QString>

namespace Hooq
{

class Communication
{
	public:
		static QString serverName();
		static QString serverName(const QString& application);
};

} // namespace
