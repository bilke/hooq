TEMPLATE = app
TARGET = hooqcli
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector

include(../common.pri)

unix:LIBS += -L../hooqInjector -lhooqInjector
win32:LIBS += "..\hooqInjector\debug\hooqInjector.lib"
QMAKE_RPATHDIR += $$(PWD)/../hooqInjector/

# Input
SOURCES += main.cpp

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