TEMPLATE = lib
TARGET = hooqui
DEPENDPATH += .
INCLUDEPATH += .

include(../common.pri)

DEFINES += HOOQ_UILIB_BUILD

HEADERS += \
	CodeEditor.h \
	CodeEditor_Margin.h \
	ColumnClickMapper.h \
	ModelIndexKeyEventObserver.h \
	PushButtonDelegate.h \
	ReplaceDialog.h \

FORMS += \
	ReplaceDialog.ui \

SOURCES += \
	CodeEditor.cpp \
	CodeEditor_Margin.cpp \
	ColumnClickMapper.cpp \
	ModelIndexKeyEventObserver.cpp \
	PushButtonDelegate.cpp \
	ReplaceDialog.cpp \

win32 {
	target.path = $${OUT_PWD}/../hooq-release
	CONFIG(debug, debug|release) {
		target.path = $${OUT_PWD}/../hooq-debug
	}
	INSTALLS += target
}

unix {
	include("../install.pri")
	target.path = $$LIBDIR
	INSTALLS += target 
}
