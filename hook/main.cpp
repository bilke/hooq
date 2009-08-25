#include "Logger.h"

extern "C" void setHooqLogFile(const char* path)
{
	Hooq::Logger::setLogFile(QString::fromLocal8Bit(path));
}

extern "C" void startHooqRecording()
{
	Hooq::Logger::activate();
}
