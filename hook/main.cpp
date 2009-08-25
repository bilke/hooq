#include "Logger.h"

extern "C" void setFredhookLogFile(const char* path)
{
	Hooq::Logger::setLogFile(QString::fromLocal8Bit(path));
}

extern "C" void registerFredhookCallback()
{
	Hooq::Logger::activate();
}
