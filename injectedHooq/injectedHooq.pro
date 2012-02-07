TEMPLATE = lib
TARGET = injectedHooq
DEPENDPATH += .
INCLUDEPATH += .

include(../common.pri)

win32:DEFINES += HOOQ_INJECTED_EXPORT=__declspec(dllexport)
win32:LIBS += user32.lib

QT += network

# Input
HEADERS += \
	../common/ObjectHookName.h \
	../common/Communication.h \
	CanaryEvent.h \
	Event.h \
	Logger.h \
	Marshall.h \
	Player.h \
	XmlPropertyDumper.h \

SOURCES += \
	../common/ObjectHookName.cpp \
	../common/Communication.cpp \
	CanaryEvent.cpp \
	Event.cpp \
	Logger.cpp \
	Marshall.cpp \
	Player.cpp \
	XmlPropertyDumper.cpp \

win32 {
	SOURCES += WinDll.cpp
	target.path = $${OUT_PWD}/../hooq-release
	CONFIG(debug, debug|release) {
		target.path = $${OUT_PWD}/../hooq-debug
	}
	INSTALLS += target
}

unix {
	SOURCES += EntryPoint.cpp

	include("../install.pri")
	target.path = $$LIBDIR
	INSTALLS += target 
}
