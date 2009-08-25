#include "GlobalEventHook.h"

/** Class that registers the hook in the constructor.
 * This is used on Windows to register the hook when
 * the dll is mapped to the appropriate process, via,
 * eg, the StartRemoteThread/LoadLibrary hack.
 *
 * As opposed to providing a DllMain, this way we
 * leave it up to the C runtime to initialise everything,
 * so it definitely won't be called before it's set up
 * properly.
 */

class GlobalEventHookLoader
{
	public:
		GlobalEventHookLoader()
		{
			GlobalEventHook::activate();
		}
};

GlobalEventHookLoader loader;