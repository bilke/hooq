TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

QT += network

# Input
HEADERS += \
	../lib/ObjectHookName.h \
	Player.h \

SOURCES += \
	../lib/ObjectHookName.cpp \
	Player.cpp \
	main.cpp \
