#include "GlobalEventHook.h"

extern "C" void setFredhookLogFile(const char* path)
{
	GlobalEventHook::setLogFile(QString::fromLocal8Bit(path));
}

extern "C" void registerFredhookCallback()
{
	GlobalEventHook::activate();
}
