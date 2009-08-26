#pragma once

#include <QtGlobal>

#ifdef Q_OS_WIN32
	#include "WindowsInjector.h"
	typedef Hooq::WindowsInjector PlatformInjector;
#else
	#include "GdbInjector.h"
	typedef Hooq::GdbInjector PlatformInjector;
#endif
