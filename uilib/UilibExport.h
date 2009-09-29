#pragma once

#include <Qt>

#ifdef Q_OS_WIN32
# ifdef HOOQ_UILIB_BUILD
#  define HOOQ_UILIB_EXPORT __declspec(dllexport)
# else
#  define HOOQ_UILIB_EXPORT __declspec(dllimport)
# endif
#else
# define HOOQ_UILIB_EXPORT
#endif