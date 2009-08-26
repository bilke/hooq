TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

QT += network

# Input
HEADERS += \
	../lib/Communication.h \
	../lib/ObjectHookName.h \
	Logger.h \

SOURCES += \
	../lib/Communication.cpp \
	../lib/ObjectHookName.cpp \
	Logger.cpp \
	main.cpp \

win32 {
	SOURCES += WinDll.cpp
}
