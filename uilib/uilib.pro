TEMPLATE = lib
TARGET = hooqui
DEPENDPATH += .
INCLUDEPATH += .

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

unix {
	include("../install.pri")
	target.path = /${DESTDIR}$$LIBDIR
	INSTALLS += target 
}
