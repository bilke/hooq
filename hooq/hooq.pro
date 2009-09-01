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
	ColumnClickMapper.h \
	EnumConverter.h \
	Interpreter.h \
	Locations.h \
	MainWindow.h \
	ModelIndexKeyEventObserver.h \
	ObjectInformation.h \
	PropertyDialog.h \
	RemoteObjectPrototype.h \
	ScriptBacktraceModel.h \
	ScriptEditor.h \
	ScriptInterface.h \
	TestFailureDialog.h \
	TestModel.h \
	TestResult.h \
	TestResultsModel.h \
	TestResultsDialog.h \
	PushButtonDelegate.h \
	VariantMapModel.h \
	XmlToQtScript.h \

FORMS += \
	MainWindow.ui \
	PropertyDialog.ui \

SOURCES += \
	BacktraceWidget.cpp \
	ColumnClickMapper.cpp \
	EnumConverter.cpp \
	Interpreter.cpp \
	Locations.cpp \
	MainWindow.cpp \
	ModelIndexKeyEventObserver.cpp \
	ObjectInformation.cpp \
	PropertyDialog.cpp \
	RemoteObjectPrototype.cpp \
	ScriptBacktraceModel.cpp \
	ScriptEditor.cpp \
	ScriptInterface.cpp \
	TestFailureDialog.cpp \
	TestModel.cpp \
	TestResult.cpp \
	TestResultsModel.cpp \
	TestResultsDialog.cpp \
	PushButtonDelegate.cpp \
	VariantMapModel.cpp \
	XmlToQtScript.cpp \
	main.cpp \

