#pragma once

#include <Qt>

#ifndef HOOQ_INJECTOR_EXPORT

#ifdef Q_OS_WIN32
#define HOOQ_INJECTOR_EXPORT __declspec(dllimport)
#else
#define HOOQ_INJECTOR_EXPORT
#endif

#endif