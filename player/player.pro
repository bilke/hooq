TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += debug

QT += network

# Input
HEADERS += \
	../lib/ObjectHookName.h \
	../lib/Communication.h \
	Player.h \

SOURCES += \
	../lib/ObjectHookName.cpp \
	../lib/Communication.cpp \
	Player.cpp \
	main.cpp \
