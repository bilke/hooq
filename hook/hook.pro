
TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

# Input
HEADERS += \
	../lib/ObjectHookName.h \
	Logger.h \

SOURCES += \
	../lib/ObjectHookName.cpp \
	Logger.cpp \
	main.cpp \

win32 {
	SOURCES += WinDll.cpp
}
