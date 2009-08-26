// Static initialiser for Hooq.
// Used on Windows because we don't have a clean way to
// call the entry point defined in EntryPoint.cpp

#include "Marshall.h"

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

class HooqLoader
{
	public:
		HooqLoader()
		{
			new Hooq::Marshall();
		}
};

HooqLoader loader;
