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
	ObjectInformation.h \
	PropertyDialog.h \
	RemoteObjectPrototype.h \
	ScriptEditor.h \
	ScriptInterface.h \
	TestModel.h \
	PushButtonDelegate.h \
	VariantMapModel.h \
	XmlToQtScript.h \

FORMS += \
	MainWindow.ui \
	PropertyDialog.ui \

SOURCES += \
	EnumConverter.cpp \
	Interpreter.cpp \
	Locations.cpp \
	MainWindow.cpp \
	ObjectInformation.cpp \
	PropertyDialog.cpp \
	RemoteObjectPrototype.cpp \
	ScriptEditor.cpp \
	ScriptInterface.cpp \
	TestModel.cpp \
	PushButtonDelegate.cpp \
	VariantMapModel.cpp \
	XmlToQtScript.cpp \
	main.cpp \

