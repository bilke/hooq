TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

QT += network

# Input
HEADERS += \
	../common/Communication.h \
	../common/ObjectHookName.h \
	Logger.h \

SOURCES += \
	../common/Communication.cpp \
	../common/ObjectHookName.cpp \
	Logger.cpp \
	main.cpp \

win32 {
	SOURCES += WinDll.cpp
}
