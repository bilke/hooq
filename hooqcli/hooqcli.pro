TEMPLATE = app
TARGET = hooqcli
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector

include(../common.pri)

CONFIG += console
CONFIG -= app_bundle

QT -= gui
QT += network

unix:LIBS += -L../hooqInjector -lhooqInjector
win32:LIBS += "..\hooqInjector\debug\hooqInjector.lib"
QMAKE_RPATHDIR += $$(PWD)/../hooqInjector/

# Input
HEADERS += ../common/Communication.h
SOURCES += \
	../common/Communication.cpp \
	main.cpp

unix {
	include("../install.pri")
	target.path = $$BINDIR
	INSTALLS += target 
}

win32 {
	target.path = $${OUT_PWD}/../hooq-release
	CONFIG(debug, debug|release) {
		target.path = $${OUT_PWD}/../hooq-debug
	}
	INSTALLS += target
}
