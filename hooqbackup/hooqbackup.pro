TEMPLATE = app
TARGET = hooqbackup
DEPENDPATH += .
INCLUDEPATH += . ../hooq

include(../common.pri)

CONFIG += console

QT -= gui

# Input
HEADERS += ../hooq/TestSetBackup.h
SOURCES += main.cpp ../hooq/TestSetBackup.cpp

unix {
	include("../install.pri")
	target.path = /${DESTDIR}$$BINDIR
	INSTALLS += target 
}

win32 {
	target.path = $${OUT_PWD}/../hooq-release
	CONFIG(debug, debug|release) {
		target.path = $${OUT_PWD}/../hooq-debug
	}
	INSTALLS += target
}