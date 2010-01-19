TEMPLATE = app
TARGET = hooq
DEPENDPATH += .
INCLUDEPATH += . ../hooqInjector/ ../uilib/ $$OUT_PWD/../uilib/

include (../common.pri)
include(../qscripthighlighter/qscripthighlighter.pri)

QMAKE_RPATHDIR += \
	$$(PWD)/../hooqInjector/ \
	$$(PWD)/../uilib/ \

unix:LIBS += \
	-L../hooqInjector -lhooqInjector \
	-L../uilib -lhooqui \

win32:LIBS += \
	"..\hooqInjector\debug\hooqInjector.lib" \
	"..\uilib\debug\hooqui.lib" \

QT += script network

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
	TestSetBackup.h \
	TestSetDialog.h \ 
	VariantMapModel.h \
	XmlToQtScript.h \
	XmlToQtScript_Item_Inserter.h \
	XmlToQtScript_FilterQtInternalsPostProcessor.h \
	XmlToQtScript_MouseMovePostProcessor.h \
	XmlToQtScript_ObjectVariablesPostProcessor.h \
	XmlToQtScript_PostProcessor.h \
	XmlToQtScript_SimplifyMouseMovementsPostProcessor.h \
	XmlToQtScript_SimplifyStringsPostProcessor.h \
	XmlToQtScript_StringVariablesPostProcessor.h \

FORMS += \
	MainWindow.ui \
	PropertyDialog.ui \
	TestSetDialog.ui \

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
	TestSetBackup.cpp \
	TestSetDialog.cpp \ 
	VariantMapModel.cpp \
	XmlToQtScript.cpp \
	XmlToQtScript_Item_Inserter.cpp \
	XmlToQtScript_FilterQtInternalsPostProcessor.cpp \
	XmlToQtScript_MouseMovePostProcessor.cpp \
	XmlToQtScript_ObjectVariablesPostProcessor.cpp \
	XmlToQtScript_PostProcessor.cpp \
	XmlToQtScript_SimplifyMouseMovementsPostProcessor.cpp \
	XmlToQtScript_SimplifyStringsPostProcessor.cpp \
	XmlToQtScript_StringVariablesPostProcessor.cpp \
	main.cpp \

unix {
	include("../install.pri")
	target.path = /${DESTDIR}$$BINDIR
	INSTALLS += target 
}

win32 {
	target.path = $${OUT_PWD}/../hooq-release
	CONFIG(debug, debug|release) {
		target.path = $${OUT_PWD}/../hooq-debug
	}
	INSTALLS += target
}