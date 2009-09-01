TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector/

LIBS += -lqscintilla2

unix:LIBS += -L../hooqInjector -lhooqInjector
win32:LIBS += "..\hooqInjector\debug\hooqInjector.lib"
QMAKE_RPATHDIR += $$(PWD)/../hooqInjector/

QT += script network

CONFIG += debug

HEADERS += \
	BacktraceWidget.h \
	EnumConverter.h \
	Interpreter.h \
	Locations.h \
	MainWindow.h \
	ObjectInformation.h \
	PropertyDialog.h \
	RemoteObjectPrototype.h \
	ScriptBacktraceModel.h \
	ScriptEditor.h \
	ScriptInterface.h \
	TestModel.h \
	TestResult.h \
	TestResultsModel.h \
	TestResultsWindow.h \
	PushButtonDelegate.h \
	VariantMapModel.h \
	XmlToQtScript.h \

FORMS += \
	MainWindow.ui \
	PropertyDialog.ui \

SOURCES += \
	BacktraceWidget.cpp \
	EnumConverter.cpp \
	Interpreter.cpp \
	Locations.cpp \
	MainWindow.cpp \
	ObjectInformation.cpp \
	PropertyDialog.cpp \
	RemoteObjectPrototype.cpp \
	ScriptBacktraceModel.cpp \
	ScriptEditor.cpp \
	ScriptInterface.cpp \
	TestModel.cpp \
	TestResult.cpp \
	TestResultsModel.cpp \
	TestResultsWindow.cpp \
	PushButtonDelegate.cpp \
	VariantMapModel.cpp \
	XmlToQtScript.cpp \
	main.cpp \

