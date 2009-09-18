TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../hooq

QT -= gui

# Input
HEADERS += ../hooq/TestSetBackup.h
SOURCES += main.cpp ../hooq/TestSetBackup.cpp

unix {
	include("../install.pri")
	target.path = /${DESTDIR}$$BINDIR
	INSTALLS += target 
}
