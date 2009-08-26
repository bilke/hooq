TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector
LIBS += -L../hooqInjector -lhooqInjector
QMAKE_RPATHDIR += $$(PWD)/../hooqInjector/

# Input
SOURCES += main.cpp
