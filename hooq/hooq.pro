TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector/

LIBS += -lqscintilla2

unix:LIBS += -L../hooqInjector -lhooqInjector
win32:LIBS += "..\hooqInjector\debug\hooqInjector.lib"
QMAKE_RPATHDIR += $$(PWD)/../hooqInjector/

QT += script scripttools network

CONFIG += debug

HEADERS += \
	EnumConverter.h \
	Interpreter.h \
	Locations.h \
	MainWindow.h \
	RemoteObjectPrototype.h \
	ScriptEditor.h \
	ScriptInterface.h \
	TestModel.h \
	TestModelDelegate.h \
	XmlToQtScript.h \

FORMS += \
	MainWindow.ui \

SOURCES += \
	EnumConverter.cpp \
	Interpreter.cpp \
	Locations.cpp \
	MainWindow.cpp \
	RemoteObjectPrototype.cpp \
	ScriptEditor.cpp \
	ScriptInterface.cpp \
	TestModel.cpp \
	TestModelDelegate.cpp \
	XmlToQtScript.cpp \
	main.cpp \

