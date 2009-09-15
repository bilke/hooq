TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector/ ../uilib/ $$OUT_PWD/../uilib/

include(../qscripthighlighter/qscripthighlighter.pri)

QMAKE_RPATHDIR += \
	$$(PWD)/../hooqInjector/ \
	$$(PWD)/../uilib/ \

unix:LIBS += \
	-L../hooqInjector -lhooqInjector \
	-L../uilib -luilib \

win32:LIBS += \
	"..\hooqInjector\debug\hooqInjector.lib" \
	"..\uilib\debug\uilib.lib" \

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
	TestFailureDialog.h \
	TestModel.h \
	TestResult.h \
	TestResultsModel.h \
	TestResultsDialog.h \
	VariantMapModel.h \
	XmlToQtScript.h \
	XmlToQtScript_MouseMovePostProcessor.h \
	XmlToQtScript_ObjectVariablesPostProcessor.h \
	XmlToQtScript_PostProcessor.h \
	XmlToQtScript_SimplifyStringsPostProcessor.h \
	XmlToQtScript_StringVariablesPostProcessor.h \

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
	TestFailureDialog.cpp \
	TestModel.cpp \
	TestResult.cpp \
	TestResultsModel.cpp \
	TestResultsDialog.cpp \
	VariantMapModel.cpp \
	XmlToQtScript.cpp \
	XmlToQtScript_MouseMovePostProcessor.cpp \
	XmlToQtScript_ObjectVariablesPostProcessor.cpp \
	XmlToQtScript_PostProcessor.cpp \
	XmlToQtScript_SimplifyStringsPostProcessor.cpp \
	XmlToQtScript_StringVariablesPostProcessor.cpp \
	main.cpp \

