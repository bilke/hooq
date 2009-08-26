TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += debug

QT += network

# Input
HEADERS += \
	../common/ObjectHookName.h \
	../common/Communication.h \
	Logger.h \
	Marshall.h \
	Player.h \

SOURCES += \
	../common/ObjectHookName.cpp \
	../common/Communication.cpp \
	Logger.cpp \
	Marshall.cpp \
	Player.cpp \

win32 {
	SOURCES += WinDll.cpp
}

unix {
	SOURCES += EntryPoint.cpp
}
