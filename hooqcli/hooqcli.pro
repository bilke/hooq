TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector

unix:LIBS += -L../hooqInjector -lhooqInjector
win32:LIBS += "..\hooqInjector\debug\hooqInjector.lib"
QMAKE_RPATHDIR += $$(PWD)/../hooqInjector/

# Input
SOURCES += main.cpp
