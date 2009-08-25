
TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

# Input
HEADERS += GlobalEventHook.h
SOURCES += GlobalEventHook.cpp main.cpp

win32 {
	SOURCES += WinDll.cpp
}
