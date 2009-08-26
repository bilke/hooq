// Provide an 'extern "C"' entry point that can be called from GDB.
// Built on Linux/OSX.
// Windows uses WinDll.cpp instead

#include "Marshall.h"

extern "C" void startHooq()
{
	new Hooq::Marshall();
}
