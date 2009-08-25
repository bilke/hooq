
TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

# Input
HEADERS += \
	../lib/ObjectHookName.h \
	GlobalEventHook.h \

SOURCES += \
	../lib/ObjectHookName.cpp \
	GlobalEventHook.cpp \
	main.cpp \

win32 {
	SOURCES += WinDll.cpp
}
