#include "InjectedImport.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace Hooq
{
	HOOQ_INJECTED_EXPORT LRESULT CALLBACK dummyHook(int nCode, WPARAM wParam, LPARAM lParam);
	extern "C" HOOQ_INJECTED_EXPORT void installHooq(HINSTANCE hMod, DWORD dwThreadId);
}