TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../lib

DEFINES += HOOQ_INJECTOR_EXPORT=__declspec(dllexport)

CONFIG += debug

QT -= gui
QT += network

# Input
HEADERS += \
	../common/Communication.h \
	Injector.h \
	RemoteLogger.h \
	RemotePlayback.h \

SOURCES += \
	../common/Communication.cpp \
	Injector.cpp \
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
