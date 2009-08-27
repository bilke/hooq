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
	XmlToQtScript.h \

FORMS += \
	MainWindow.ui \

SOURCES += \
	Locations.cpp \
	MainWindow.cpp \
	TestModel.cpp \
	XmlToQtScript.cpp \
	main.cpp \

