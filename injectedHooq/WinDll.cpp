/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
// Static initialiser for Hooq.
// Used on Windows because we don't have a clean way to
// call the entry point defined in EntryPoint.cpp

#include "Marshall.h"
#include "WinDll.h"

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

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <QCoreApplication>
#include <QDebug>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		qDebug() << Q_FUNC_INFO << QCoreApplication::applicationFilePath();
		// Increase reference count
		wchar_t path[MAX_PATH];
		::GetModuleFileNameW(static_cast<HMODULE>(hModule), path, MAX_PATH);
		::LoadLibrary(path);
//		new Hooq::Marshall();

		// Remove hook
		//::UnhookWindowsHookEx(g_hHook);
	}
	return TRUE;
}


namespace Hooq
{
LRESULT CALLBACK dummyHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	Marshall::instance();
	return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}

void installHooq(HINSTANCE hMod, DWORD dwThreadId)
{
	::SetWindowsHookEx(WH_CALLWNDPROC, dummyHook, hMod, dwThreadId);
}

}