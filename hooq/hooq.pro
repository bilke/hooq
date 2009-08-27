TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector/

unix:LIBS += -L../hooqInjector -lhooqInjector
win32:LIBS += "..\hooqInjector\debug\hooqInjector.lib"
QMAKE_RPATHDIR += $$(PWD)/../hooqInjector/

CONFIG += debug

HEADERS += \
	Locations.h \
	MainWindow.h \
	TestModel.h \

FORMS += \
	MainWindow.ui \

SOURCES += \
	Locations.cpp \
	MainWindow.cpp \
	TestModel.cpp \
	main.cpp \

