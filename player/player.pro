TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

QT += network

# Input
HEADERS += \
	../common/ObjectHookName.h \
	../common/Communication.h \
	Player.h \

SOURCES += \
	../common/ObjectHookName.cpp \
	../common/Communication.cpp \
	Player.cpp \
	main.cpp \
