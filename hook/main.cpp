#include "GlobalEventHook.h"

extern "C" void registerFredhookCallback()
{
	GlobalEventHook::activate();
}
