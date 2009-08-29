TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../lib

win32:DEFINES += HOOQ_INJECTOR_EXPORT=__declspec(dllexport)
win32:LIBS += user32.lib

CONFIG += debug

QT -= gui
QT += network

# Input
HEADERS += \
	../common/Communication.h \
	Injector.h \
	RemoteConnection.h \
	RemoteLogger.h \
	RemotePlayback.h \

SOURCES += \
	../common/Communication.cpp \
	Injector.cpp \
	RemoteConnection.cpp \
	RemoteLogger.cpp \
	RemotePlayback.cpp \

win32 {
	HEADERS += WindowsInjector.h
	SOURCES += WindowsInjector.cpp
}

unix {
	HEADERS += GdbInjector.h
	SOURCES += GdbInjector.cpp
}
