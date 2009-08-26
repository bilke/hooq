#include "Logger.h"

extern "C" void startHooqRecording()
{
	Hooq::Logger::activate();
}
