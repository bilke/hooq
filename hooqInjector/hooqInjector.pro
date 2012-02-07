TEMPLATE = lib
TARGET = hooqInjector
DEPENDPATH += .
INCLUDEPATH += . ../lib

win32:DEFINES += HOOQ_INJECTOR_EXPORT=__declspec(dllexport)
win32:LIBS += user32.lib

include(../common.pri)

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
	target.path = $${OUT_PWD}/../hooq-release
	CONFIG(debug, debug|release) {
		target.path = $${OUT_PWD}/../hooq-debug
	}
	INSTALLS += target
}

unix {
	HEADERS += GdbInjector.h
	SOURCES += GdbInjector.cpp

	include("../install.pri")
	target.path = $$LIBDIR
	INSTALLS += target 
}